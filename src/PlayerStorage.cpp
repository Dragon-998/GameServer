#include "PlayerStorage.h"

#include "Player.h"

#include <filesystem>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <winsqlite/winsqlite3.h>
#else
#include <sqlite3.h>
#endif

namespace {

std::string databaseError(sqlite3* database, const char* operation) {
    return std::string(operation) + " failed: " + sqlite3_errmsg(database);
}

void checkSqlResult(sqlite3* database, int result, const char* operation) {
    if (result != SQLITE_OK) {
        throw std::runtime_error(databaseError(database, operation));
    }
}

sqlite3_int64 toSqlitePlayerId(PlayerId playerId) {
    const auto maxSqliteId = static_cast<PlayerId>(
        std::numeric_limits<sqlite3_int64>::max());
    if (playerId > maxSqliteId) {
        throw std::invalid_argument("playerId exceeds SQLite INTEGER range");
    }
    return static_cast<sqlite3_int64>(playerId);
}

using Statement = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

Statement prepareStatement(sqlite3* database, const char* sql) {
    sqlite3_stmt* rawStatement = nullptr;
    const int result = sqlite3_prepare_v2(database, sql, -1, &rawStatement, nullptr);
    Statement statement(rawStatement, sqlite3_finalize);
    checkSqlResult(database, result, "sqlite3_prepare_v2");
    return statement;
}

}  // namespace

PlayerStorage::PlayerStorage(const std::string& databasePath) {
    const std::filesystem::path path(databasePath);
    if (databasePath != ":memory:" && !path.parent_path().empty()) {
        std::filesystem::create_directories(path.parent_path());
    }

    const int openResult = sqlite3_open(databasePath.c_str(), &database_);
    if (openResult != SQLITE_OK) {
        const std::string error = database_ == nullptr
                                      ? "sqlite3_open failed"
                                      : databaseError(database_, "sqlite3_open");
        if (database_ != nullptr) {
            sqlite3_close(database_);
            database_ = nullptr;
        }
        throw std::runtime_error(error);
    }

    const char* createTableSql =
        "CREATE TABLE IF NOT EXISTS players ("
        "player_id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL);";
    char* errorMessage = nullptr;
    const int schemaResult =
        sqlite3_exec(database_, createTableSql, nullptr, nullptr, &errorMessage);
    if (schemaResult != SQLITE_OK) {
        const std::string error =
            errorMessage == nullptr ? databaseError(database_, "create players table")
                                   : std::string("create players table failed: ") +
                                         errorMessage;
        sqlite3_free(errorMessage);
        sqlite3_close(database_);
        database_ = nullptr;
        throw std::runtime_error(error);
    }
}

PlayerStorage::~PlayerStorage() {
    if (database_ != nullptr) {
        sqlite3_close(database_);
    }
}

void PlayerStorage::save(const Player& player) {
    const char* sql =
        "INSERT INTO players (player_id, name) VALUES (?1, ?2) "
        "ON CONFLICT(player_id) DO UPDATE SET name = excluded.name;";
    Statement statement = prepareStatement(database_, sql);

    checkSqlResult(database_,
                   sqlite3_bind_int64(statement.get(), 1,
                                      toSqlitePlayerId(player.playerId())),
                   "sqlite3_bind_int64");

    if (player.name().size() >
        static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("player name is too long for SQLite");
    }
    checkSqlResult(database_,
                   sqlite3_bind_text(statement.get(), 2, player.name().c_str(),
                                     static_cast<int>(player.name().size()),
                                     SQLITE_TRANSIENT),
                   "sqlite3_bind_text");

    const int stepResult = sqlite3_step(statement.get());
    if (stepResult != SQLITE_DONE) {
        throw std::runtime_error(databaseError(database_, "save Player"));
    }
}

std::unique_ptr<Player> PlayerStorage::load(PlayerId playerId) const {
    const char* sql =
        "SELECT player_id, name FROM players WHERE player_id = ?1;";
    Statement statement = prepareStatement(database_, sql);
    checkSqlResult(database_,
                   sqlite3_bind_int64(statement.get(), 1,
                                      toSqlitePlayerId(playerId)),
                   "sqlite3_bind_int64");

    const int stepResult = sqlite3_step(statement.get());
    if (stepResult == SQLITE_DONE) {
        return nullptr;
    }
    if (stepResult != SQLITE_ROW) {
        throw std::runtime_error(databaseError(database_, "load Player"));
    }

    const sqlite3_int64 storedId = sqlite3_column_int64(statement.get(), 0);
    const auto* storedName = sqlite3_column_text(statement.get(), 1);
    const int nameLength = sqlite3_column_bytes(statement.get(), 1);
    if (storedId < 0 || storedName == nullptr || nameLength < 0) {
        throw std::runtime_error("players table contains invalid Player data");
    }

    std::string name(reinterpret_cast<const char*>(storedName),
                     static_cast<std::size_t>(nameLength));
    return std::make_unique<Player>(static_cast<PlayerId>(storedId),
                                    std::move(name));
}

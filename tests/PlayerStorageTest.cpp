#include "Player.h"
#include "PlayerStorage.h"

#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>

namespace {

class TemporaryDatabaseFile {
public:
    TemporaryDatabaseFile() {
        const auto timestamp = std::chrono::steady_clock::now()
                                  .time_since_epoch()
                                  .count();
        path_ = std::filesystem::temp_directory_path() /
                ("MiniGameServer_PlayerStorageTest_" +
                 std::to_string(timestamp) + ".db");
    }

    ~TemporaryDatabaseFile() {
        std::error_code ignoredError;
        std::filesystem::remove(path_, ignoredError);
    }

    [[nodiscard]] const std::filesystem::path& path() const noexcept {
        return path_;
    }

private:
    std::filesystem::path path_;
};

bool verify(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << std::endl;
    }
    return condition;
}

}  // namespace

int main() {
    try {
        TemporaryDatabaseFile databaseFile;
        constexpr PlayerId kPlayerId = 10001;

        {
            PlayerStorage storage(databaseFile.path().string());
            if (!verify(storage.load(kPlayerId) == nullptr,
                        "A new database should not contain the test Player.")) {
                return 1;
            }

            Player player(kPlayerId, "test");
            storage.save(player);
            const std::unique_ptr<Player> loadedPlayer = storage.load(kPlayerId);
            if (!verify(loadedPlayer != nullptr &&
                            loadedPlayer->playerId() == kPlayerId &&
                            loadedPlayer->name() == "test",
                        "PlayerStorage failed to save and load a Player.")) {
                return 1;
            }
        }

        {
            PlayerStorage storage(databaseFile.path().string());
            std::unique_ptr<Player> loadedPlayer = storage.load(kPlayerId);
            if (!verify(loadedPlayer != nullptr && loadedPlayer->name() == "test",
                        "PlayerStorage failed to load after reopening the database.")) {
                return 1;
            }

            loadedPlayer->setName("renamed");
            storage.save(*loadedPlayer);
        }

        {
            PlayerStorage storage(databaseFile.path().string());
            const std::unique_ptr<Player> renamedPlayer = storage.load(kPlayerId);
            if (!verify(renamedPlayer != nullptr &&
                            renamedPlayer->name() == "renamed",
                        "PlayerStorage failed to persist an updated Player name.") ||
                !verify(storage.load(99999) == nullptr,
                        "Loading an unknown Player should return nullptr.")) {
                return 1;
            }
        }

        std::cout << "PlayerStorage tests passed." << std::endl;
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "PlayerStorage test failed: " << exception.what()
                  << std::endl;
        return 1;
    }
}

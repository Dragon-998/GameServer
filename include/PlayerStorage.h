#pragma once

#include "PlayerId.h"

#include <memory>
#include <string>

class Player;
struct sqlite3;

class PlayerStorage {
public:
    explicit PlayerStorage(const std::string& databasePath);
    ~PlayerStorage();

    PlayerStorage(const PlayerStorage&) = delete;
    PlayerStorage& operator=(const PlayerStorage&) = delete;

    void save(const Player& player);
    [[nodiscard]] std::unique_ptr<Player> load(PlayerId playerId) const;

private:
    sqlite3* database_{nullptr};
};

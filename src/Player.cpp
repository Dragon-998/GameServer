#include "Player.h"

#include <utility>

Player::Player(PlayerId playerId, std::string name)
    : playerId_(playerId), name_(std::move(name)) {}

PlayerId Player::playerId() const noexcept {
    return playerId_;
}

const std::string& Player::name() const noexcept {
    return name_;
}

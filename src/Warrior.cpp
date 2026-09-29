#include "Warrior.h"

#include <algorithm>
#include <utility>

Warrior::Warrior(WarriorId warriorId,
                 std::optional<PlayerId> ownerPlayerId,
                 EntityId sourceEntityId, int hp, int attack, int defense,
                 int speed)
    : warriorId_(warriorId), ownerPlayerId_(std::move(ownerPlayerId)),
      sourceEntityId_(sourceEntityId), hp_(std::max(0, hp)), attack_(attack),
      defense_(defense), speed_(speed) {}

WarriorId Warrior::warriorId() const noexcept {
    return warriorId_;
}

const std::optional<PlayerId>& Warrior::ownerPlayerId() const noexcept {
    return ownerPlayerId_;
}

EntityId Warrior::sourceEntityId() const noexcept {
    return sourceEntityId_;
}

int Warrior::hp() const noexcept {
    return hp_;
}

int Warrior::attack() const noexcept {
    return attack_;
}

int Warrior::defense() const noexcept {
    return defense_;
}

int Warrior::speed() const noexcept {
    return speed_;
}

bool Warrior::isAlive() const noexcept {
    return hp_ > 0;
}

int Warrior::takeDamage(int damage) noexcept {
    if (damage <= 0 || !isAlive()) {
        return 0;
    }

    const int appliedDamage = std::min(damage, hp_);
    hp_ -= appliedDamage;
    return appliedDamage;
}

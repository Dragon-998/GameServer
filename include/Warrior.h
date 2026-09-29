#pragma once

#include "BattleReport.h"
#include "PlayerId.h"
#include "WorldTypes.h"

#include <optional>

class Warrior {
public:
    static constexpr int defaultHp = 100;
    static constexpr int defaultAttack = 20;
    static constexpr int defaultDefense = 5;
    static constexpr int defaultSpeed = 10;

    Warrior(WarriorId warriorId, std::optional<PlayerId> ownerPlayerId,
            EntityId sourceEntityId, int hp = defaultHp,
            int attack = defaultAttack, int defense = defaultDefense,
            int speed = defaultSpeed);

    [[nodiscard]] WarriorId warriorId() const noexcept;
    [[nodiscard]] const std::optional<PlayerId>& ownerPlayerId() const noexcept;
    [[nodiscard]] EntityId sourceEntityId() const noexcept;
    [[nodiscard]] int hp() const noexcept;
    [[nodiscard]] int attack() const noexcept;
    [[nodiscard]] int defense() const noexcept;
    [[nodiscard]] int speed() const noexcept;
    [[nodiscard]] bool isAlive() const noexcept;

    int takeDamage(int damage) noexcept;

private:
    WarriorId warriorId_;
    std::optional<PlayerId> ownerPlayerId_;
    EntityId sourceEntityId_;
    int hp_;
    int attack_;
    int defense_;
    int speed_;
};

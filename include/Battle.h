#pragma once

#include "BattleReport.h"
#include "Warrior.h"

#include <vector>

class Battle {
public:
    explicit Battle(BattleId battleId);

    [[nodiscard]] bool addWarrior(Warrior warrior);
    [[nodiscard]] bool start();

    [[nodiscard]] const BattleReport& report() const noexcept;
    [[nodiscard]] const std::vector<Warrior>& warriors() const noexcept;

private:
    [[nodiscard]] Warrior* findWarrior(WarriorId warriorId) noexcept;

    BattleId battleId_;
    std::vector<Warrior> warriors_;
    std::uint32_t currentRound_{0};
    BattleReport report_;
    bool started_{false};
};

#pragma once

class Warrior;

class BasicAttackSkill {
public:
    [[nodiscard]] int execute(Warrior& attacker, Warrior& target) const noexcept;
};

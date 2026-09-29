#include "BasicAttackSkill.h"

#include "Warrior.h"

#include <algorithm>
#include <limits>

int BasicAttackSkill::execute(Warrior& attacker,
                              Warrior& target) const noexcept {
    if (!attacker.isAlive() || !target.isAlive() || &attacker == &target) {
        return 0;
    }

    const long long rawDamage = static_cast<long long>(attacker.attack()) -
                                static_cast<long long>(target.defense());
    const long long boundedDamage =
        std::max(1LL, std::min(rawDamage,
                               static_cast<long long>(
                                   std::numeric_limits<int>::max())));
    const int damage = static_cast<int>(boundedDamage);
    return target.takeDamage(damage);
}

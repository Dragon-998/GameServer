#include "Battle.h"

#include "BasicAttackSkill.h"

#include <algorithm>
#include <utility>

Battle::Battle(BattleId battleId)
    : battleId_(battleId), report_(battleId) {
    warriors_.reserve(2);
}

bool Battle::addWarrior(Warrior warrior) {
    if (started_ || warriors_.size() >= 2 ||
        findWarrior(warrior.warriorId()) != nullptr) {
        return false;
    }

    warriors_.push_back(std::move(warrior));
    return true;
}

bool Battle::start() {
    if (started_ || warriors_.size() != 2 || !warriors_[0].isAlive() ||
        !warriors_[1].isAlive()) {
        return false;
    }
    started_ = true;

    std::vector<WarriorId> turnOrder;
    turnOrder.reserve(warriors_.size());
    for (const Warrior& warrior : warriors_) {
        turnOrder.push_back(warrior.warriorId());
    }

    std::sort(turnOrder.begin(), turnOrder.end(), [this](WarriorId leftId,
                                                        WarriorId rightId) {
        const Warrior* left = findWarrior(leftId);
        const Warrior* right = findWarrior(rightId);
        if (left == nullptr || right == nullptr) {
            return leftId < rightId;
        }
        if (left->speed() != right->speed()) {
            return left->speed() > right->speed();
        }
        return leftId < rightId;
    });

    BasicAttackSkill basicAttack;
    while (true) {
        ++currentRound_;
        report_.recordRoundStart(currentRound_);

        for (const WarriorId attackerId : turnOrder) {
            Warrior* attacker = findWarrior(attackerId);
            if (attacker == nullptr) {
                return false;
            }
            if (!attacker->isAlive()) {
                continue;
            }

            Warrior* target = nullptr;
            for (Warrior& candidate : warriors_) {
                if (candidate.warriorId() != attackerId &&
                    candidate.isAlive()) {
                    target = &candidate;
                    break;
                }
            }
            if (target == nullptr) {
                report_.recordBattleEnd(currentRound_, attackerId);
                return true;
            }

            const int damage = basicAttack.execute(*attacker, *target);
            if (damage <= 0) {
                return false;
            }
            report_.recordAttack(currentRound_, attackerId,
                                 target->warriorId(), damage, target->hp());

            if (!target->isAlive()) {
                report_.recordBattleEnd(currentRound_, attackerId);
                return true;
            }
        }
    }
}

const BattleReport& Battle::report() const noexcept {
    return report_;
}

const std::vector<Warrior>& Battle::warriors() const noexcept {
    return warriors_;
}

Warrior* Battle::findWarrior(WarriorId warriorId) noexcept {
    const auto found = std::find_if(
        warriors_.begin(), warriors_.end(), [warriorId](const Warrior& warrior) {
            return warrior.warriorId() == warriorId;
        });
    return found == warriors_.end() ? nullptr : &*found;
}

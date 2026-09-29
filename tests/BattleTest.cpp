#include "Battle.h"
#include "BasicAttackSkill.h"
#include "Warrior.h"

#include <iostream>
#include <optional>

namespace {

bool verify(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << std::endl;
    }
    return condition;
}

}  // namespace

int main() {
    Warrior attacker(1, PlayerId{10001}, EntityId{7});
    Warrior target(2, std::nullopt, EntityId{1});
    if (!verify(attacker.hp() == 100 && attacker.attack() == 20 &&
                    attacker.defense() == 5 && attacker.speed() == 10,
                "Warrior did not use the expected default attributes.") ||
        !verify(target.hp() == 100,
                "The target Warrior should start with 100 HP.")) {
        return 1;
    }

    BasicAttackSkill basicAttack;
    const int damage = basicAttack.execute(attacker, target);
    if (!verify(damage == 15 && target.hp() == 85,
                "BasicAttackSkill should deal attack - defense damage.")) {
        return 1;
    }

    Battle battle(7);
    if (!verify(battle.addWarrior(Warrior(1, PlayerId{10001}, EntityId{7})) &&
                    battle.addWarrior(Warrior(2, std::nullopt, EntityId{1})) &&
                    battle.start(),
                "A valid 1v1 battle failed to start.")) {
        return 1;
    }

    const BattleReport& report = battle.report();
    const auto& events = report.events();
    if (!verify(report.battleId() == BattleId{7} &&
                    report.winnerId() == WarriorId{1},
                "BattleReport did not record the battle ID and winner.") ||
        !verify(!events.empty() &&
                    events.front().type == BattleEventType::RoundStart,
                "BattleReport did not record the first round.")) {
        return 1;
    }

    int attackCount = 0;
    int roundStartCount = 0;
    bool defeatedWarriorActedInFinalRound = false;
    bool finalAttackDefeatedTarget = false;
    for (const BattleEvent& event : events) {
        if (event.type == BattleEventType::RoundStart) {
            ++roundStartCount;
        } else if (event.type == BattleEventType::Attack) {
            ++attackCount;
            if (event.round == 7 && event.attackerId == 2) {
                defeatedWarriorActedInFinalRound = true;
            }
            if (event.round == 7 && event.attackerId == 1 &&
                event.targetId == 2 && event.remainingHp == 0) {
                finalAttackDefeatedTarget = true;
            }
        }
    }

    if (!verify(attackCount == 13 && roundStartCount == 7,
                "BattleReport did not record each round and attack.") ||
        !verify(finalAttackDefeatedTarget &&
                    !defeatedWarriorActedInFinalRound,
                "A defeated Warrior acted again or the final hit was not recorded.") ||
        !verify(events.back().type == BattleEventType::BattleEnd &&
                    events.back().winnerId == WarriorId{1},
                "BattleReport did not record the battle result.")) {
        return 1;
    }

    Battle speedBattle(8);
    if (!verify(speedBattle.addWarrior(
                    Warrior(1, PlayerId{10001}, EntityId{7}, 100, 20, 5, 5)) &&
                    speedBattle.addWarrior(
                        Warrior(2, std::nullopt, EntityId{1}, 100, 20, 5, 15)) &&
                    speedBattle.start(),
                "Battle with different Warrior speeds failed to start.")) {
        return 1;
    }
    const auto& speedEvents = speedBattle.report().events();
    if (!verify(speedEvents.size() > 1 &&
                    speedEvents[1].type == BattleEventType::Attack &&
                    speedEvents[1].attackerId == 2,
                "The faster Warrior did not act first.")) {
        return 1;
    }

    Battle incompleteBattle(9);
    if (!verify(incompleteBattle.addWarrior(
                    Warrior(1, PlayerId{10001}, EntityId{7})) &&
                    !incompleteBattle.start(),
                "Battle should reject a missing second Warrior safely.")) {
        return 1;
    }

    std::cout << "Battle tests passed." << std::endl;
    return 0;
}

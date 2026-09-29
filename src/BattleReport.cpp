#include "BattleReport.h"

#include <utility>

BattleReport::BattleReport(BattleId battleId) : battleId_(battleId) {}

void BattleReport::recordRoundStart(std::uint32_t round) {
    BattleEvent event;
    event.type = BattleEventType::RoundStart;
    event.round = round;
    events_.push_back(std::move(event));
}

void BattleReport::recordAttack(std::uint32_t round, WarriorId attackerId,
                                WarriorId targetId, int damage,
                                int remainingHp) {
    BattleEvent event;
    event.type = BattleEventType::Attack;
    event.round = round;
    event.attackerId = attackerId;
    event.targetId = targetId;
    event.damage = damage;
    event.remainingHp = remainingHp;
    events_.push_back(std::move(event));
}

void BattleReport::recordBattleEnd(std::uint32_t round, WarriorId winnerId) {
    if (winnerId_.has_value()) {
        return;
    }

    winnerId_ = winnerId;
    BattleEvent event;
    event.type = BattleEventType::BattleEnd;
    event.round = round;
    event.winnerId = winnerId;
    events_.push_back(std::move(event));
}

BattleId BattleReport::battleId() const noexcept {
    return battleId_;
}

const std::vector<BattleEvent>& BattleReport::events() const noexcept {
    return events_;
}

std::optional<WarriorId> BattleReport::winnerId() const noexcept {
    return winnerId_;
}

#pragma once

#include <cstdint>
#include <optional>
#include <vector>

using BattleId = std::uint64_t;
using WarriorId = std::uint64_t;

enum class BattleEventType {
    RoundStart,
    Attack,
    BattleEnd,
};

struct BattleEvent {
    BattleEventType type{BattleEventType::RoundStart};
    std::uint32_t round{0};
    WarriorId attackerId{0};
    WarriorId targetId{0};
    int damage{0};
    int remainingHp{0};
    std::optional<WarriorId> winnerId;
};

class BattleReport {
public:
    explicit BattleReport(BattleId battleId);

    void recordRoundStart(std::uint32_t round);
    void recordAttack(std::uint32_t round, WarriorId attackerId,
                      WarriorId targetId, int damage, int remainingHp);
    void recordBattleEnd(std::uint32_t round, WarriorId winnerId);

    [[nodiscard]] BattleId battleId() const noexcept;
    [[nodiscard]] const std::vector<BattleEvent>& events() const noexcept;
    [[nodiscard]] std::optional<WarriorId> winnerId() const noexcept;

private:
    BattleId battleId_;
    std::vector<BattleEvent> events_;
    std::optional<WarriorId> winnerId_;
};

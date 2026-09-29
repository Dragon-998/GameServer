#include "MoveHandler.h"

#include "PacketCodec.h"
#include "Player.h"
#include "PlayerEntity.h"
#include "PlayerManager.h"
#include "ProtocolId.h"
#include "Session.h"
#include "World.h"

#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

bool parseInteger(std::string_view text, int& value) {
    if (text.empty()) {
        return false;
    }

    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto result = std::from_chars(begin, end, value);
    return result.ec == std::errc{} && result.ptr == end;
}

bool parseMoveRequest(const Packet& packet, int& dx, int& dy) {
    const std::string payload(packet.data.begin(), packet.data.end());
    constexpr std::string_view prefix = "dx=";
    constexpr std::string_view yMarker = "&dy=";

    if (payload.compare(0, prefix.size(), prefix) != 0) {
        return false;
    }

    const std::size_t yPosition = payload.find(yMarker, prefix.size());
    if (yPosition == std::string::npos) {
        return false;
    }

    return parseInteger(
               std::string_view(payload).substr(prefix.size(),
                                                yPosition - prefix.size()),
               dx) &&
           parseInteger(std::string_view(payload).substr(yPosition + yMarker.size()),
                        dy);
}

void sendMoveResponse(Session& session, bool success, const std::string& message,
                      int x = 0, int y = 0) {
    Packet response;
    response.protocolId =
        static_cast<std::uint16_t>(ProtocolId::MoveResponse);

    const std::string payload = success
                                    ? "success=1&x=" + std::to_string(x) +
                                          "&y=" + std::to_string(y)
                                    : "success=0&message=" + message;
    response.data.assign(payload.begin(), payload.end());

    const std::vector<std::uint8_t> bytes = PacketCodec::encode(response);
    if (session.Send(bytes) < 0) {
        std::cerr << "Move response send failed." << std::endl;
    }
}

}  // namespace

MoveHandler::MoveHandler(PlayerManager& playerManager, World& world)
    : playerManager_(playerManager), world_(world) {}

void MoveHandler::handle(const Packet& packet, Session& session) {
    std::cout << "MoveHandler received packet" << std::endl;

    int dx = 0;
    int dy = 0;
    if (!parseMoveRequest(packet, dx, dy)) {
        sendMoveResponse(session, false, "invalid request format");
        return;
    }

    if (dx < -1 || dx > 1 || dy < -1 || dy > 1 || (dx == 0 && dy == 0)) {
        sendMoveResponse(session, false, "invalid movement delta");
        return;
    }

    const std::optional<PlayerId> playerId = session.playerId();
    if (!playerId.has_value()) {
        sendMoveResponse(session, false, "not logged in");
        return;
    }

    Player* player = playerManager_.getPlayer(*playerId);
    if (player == nullptr) {
        sendMoveResponse(session, false, "player not found");
        return;
    }

    PlayerEntity* playerEntity = world_.findPlayerEntity(*player);
    if (playerEntity == nullptr) {
        sendMoveResponse(session, false, "player entity not found");
        return;
    }

    if (!playerEntity->moveBy(dx, dy)) {
        sendMoveResponse(session, false, "position out of range");
        return;
    }

    sendMoveResponse(session, true, "", playerEntity->x(), playerEntity->y());
    std::cout << "Player " << *playerId << " moved to (" << playerEntity->x()
              << ", " << playerEntity->y() << ")" << std::endl;
}

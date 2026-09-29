#include "LoginHandler.h"

#include "LoginMessage.h"
#include "PacketCodec.h"
#include "Player.h"
#include "PlayerManager.h"
#include "ProtocolId.h"
#include "Session.h"

#include <iostream>
#include <string>

namespace {

bool parseLoginRequest(const Packet& packet, LoginRequest& request) {
    const std::string payload(packet.data.begin(), packet.data.end());
    constexpr char usernamePrefix[] = "username=";
    constexpr char passwordMarker[] = "&password=";

    if (payload.compare(0, sizeof(usernamePrefix) - 1, usernamePrefix) != 0) {
        return false;
    }

    const std::size_t passwordPosition =
        payload.find(passwordMarker, sizeof(usernamePrefix) - 1);
    if (passwordPosition == std::string::npos) {
        return false;
    }

    const std::size_t passwordStart =
        passwordPosition + sizeof(passwordMarker) - 1;
    request.username = payload.substr(sizeof(usernamePrefix) - 1,
                                      passwordPosition -
                                          (sizeof(usernamePrefix) - 1));
    request.password = payload.substr(passwordStart);

    if (request.username.empty() || request.password.empty() ||
        request.username.find_first_of("&=") != std::string::npos ||
        request.password.find_first_of("&=") != std::string::npos) {
        return false;
    }

    return true;
}

Packet makeLoginResponsePacket(const LoginResponse& response) {
    Packet packet;
    packet.protocolId = static_cast<std::uint16_t>(ProtocolId::LoginResponse);

    const std::string payload =
        std::string("success=") + (response.success ? "1" : "0") +
        "&message=" + response.message;
    packet.data.assign(payload.begin(), payload.end());
    return packet;
}

}  // namespace

LoginHandler::LoginHandler(PlayerManager& playerManager)
    : playerManager_(playerManager) {}

void LoginHandler::handle(const Packet& packet, Session& session) {
    std::cout << "LoginHandler received packet" << std::endl;

    LoginRequest request;
    LoginResponse response;

    if (!parseLoginRequest(packet, request)) {
        response.success = false;
        response.message = "invalid request format";
    } else if (request.username == "test" && request.password == "123456") {
        if (session.hasPlayer()) {
            response.success = false;
            response.message = "session already bound to a player";
        } else {
            constexpr PlayerId kTestPlayerId = 10001;
            Player* player = playerManager_.getPlayer(kTestPlayerId);
            if (player == nullptr) {
                player = playerManager_.addPlayer(kTestPlayerId, request.username);
            }

            if (player != nullptr && session.bindPlayer(player->playerId())) {
                response.success = true;
                response.message = "login success";
                std::cout << "Login success" << std::endl;
                std::cout << "Session " << session.sessionId()
                          << " bound to Player " << player->playerId()
                          << std::endl;
            } else {
                response.success = false;
                response.message = "player unavailable for this session";
            }
        }
    } else {
        response.success = false;
        response.message = "login failed";
    }

    const Packet responsePacket = makeLoginResponsePacket(response);
    const std::vector<std::uint8_t> responseBytes =
        PacketCodec::encode(responsePacket);
    if (session.Send(responseBytes) < 0) {
        std::cerr << "Login response send failed." << std::endl;
        return;
    }

    std::cout << "LoginHandler sent: " << response.message << std::endl;
}

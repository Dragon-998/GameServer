#include "Dispatcher.h"

#include "PlayerManager.h"
#include "PlayerEntity.h"
#include "PlayerStorage.h"
#include "ProtocolId.h"
#include "Session.h"
#include "World.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

namespace {

bool dispatchPrints(Dispatcher& dispatcher, Session& session,
                    std::uint16_t protocolId,
                    const std::string& expectedOutput) {
    Packet packet;
    packet.protocolId = protocolId;

    std::ostringstream capturedOutput;
    std::ostringstream capturedError;
    std::streambuf* originalOutput = std::cout.rdbuf(capturedOutput.rdbuf());
    std::streambuf* originalError = std::cerr.rdbuf(capturedError.rdbuf());
    dispatcher.dispatch(packet, session);
    std::cout.rdbuf(originalOutput);
    std::cerr.rdbuf(originalError);

    if (capturedOutput.str() != expectedOutput) {
        std::cerr << "Protocol " << protocolId << " produced unexpected output."
                  << " Expected: " << expectedOutput
                  << " Actual: " << capturedOutput.str();
        return false;
    }

    return true;
}

void dispatchLogin(Dispatcher& dispatcher, Session& session,
                   const std::string& request) {
    Packet packet;
    packet.protocolId = static_cast<std::uint16_t>(ProtocolId::Login);
    packet.data.assign(request.begin(), request.end());

    std::ostringstream capturedOutput;
    std::streambuf* originalOutput = std::cout.rdbuf(capturedOutput.rdbuf());
    std::streambuf* originalError = std::cerr.rdbuf(capturedOutput.rdbuf());
    dispatcher.dispatch(packet, session);
    std::cout.rdbuf(originalOutput);
    std::cerr.rdbuf(originalError);
}

}  // namespace

int main() {
    PlayerManager managerTest;
    if (managerTest.hasPlayer(10001) ||
        managerTest.getPlayer(10001) != nullptr) {
        std::cerr << "PlayerManager should start empty." << std::endl;
        return 1;
    }

    Player* addedPlayer = managerTest.addPlayer(10001, "test");
    if (addedPlayer == nullptr || !managerTest.hasPlayer(10001) ||
        managerTest.getPlayer(10001) != addedPlayer ||
        addedPlayer->playerId() != PlayerId{10001} ||
        addedPlayer->name() != "test") {
        std::cerr << "PlayerManager failed to add or find a Player."
                  << std::endl;
        return 1;
    }

    if (managerTest.addPlayer(10001, "duplicate") != nullptr ||
        managerTest.getPlayer(99999) != nullptr ||
        managerTest.hasPlayer(99999)) {
        std::cerr << "PlayerManager handled a duplicate or unknown ID "
                     "incorrectly."
                  << std::endl;
        return 1;
    }

    PlayerManager loginPlayerManager;
    PlayerStorage playerStorage(":memory:");
    World world;
    Dispatcher dispatcher(loginPlayerManager, playerStorage, world);
    Session session(kInvalidSocket, 1);

    if (session.hasPlayer() || session.playerId().has_value()) {
        std::cerr << "A new Session should not be bound to a Player."
                  << std::endl;
        return 1;
    }

    const bool loginWorks = dispatchPrints(
        dispatcher, session, static_cast<std::uint16_t>(ProtocolId::Login),
        "LoginHandler received packet\n");
    const bool moveWorks = dispatchPrints(
        dispatcher, session, static_cast<std::uint16_t>(ProtocolId::Move),
        "MoveHandler received packet\n");
    const bool fightWorks = dispatchPrints(
        dispatcher, session, static_cast<std::uint16_t>(ProtocolId::Fight),
        "FightHandler received packet\n");
    const bool unknownWorks = dispatchPrints(
        dispatcher, session, 9999, "Unknown protocol id: 9999\n");

    if (!loginWorks || !moveWorks || !fightWorks || !unknownWorks) {
        return 1;
    }

    Session successfulLoginSession(kInvalidSocket, 2);
    dispatchLogin(dispatcher, successfulLoginSession,
                  "username=test&password=123456");
    const Player* player = loginPlayerManager.getPlayer(10001);
    if (!successfulLoginSession.hasPlayer() ||
        successfulLoginSession.playerId() != PlayerId{10001} ||
        player == nullptr || player->name() != "test" ||
        !loginPlayerManager.hasPlayer(10001)) {
        std::cerr << "Successful login did not bind Player 10001."
                  << std::endl;
        return 1;
    }

    const auto& entityLocation = player->entityLocation();
    if (!entityLocation.has_value() ||
        entityLocation->mapId != World::defaultMapId ||
        world.findPlayerEntity(*player) == nullptr) {
        std::cerr << "Successful login did not place Player 10001 in the World."
                  << std::endl;
        return 1;
    }

    Packet movePacket;
    movePacket.protocolId = static_cast<std::uint16_t>(ProtocolId::Move);
    const std::string moveRequest = "dx=1&dy=0";
    movePacket.data.assign(moveRequest.begin(), moveRequest.end());
    dispatcher.dispatch(movePacket, successfulLoginSession);

    const PlayerEntity* movedEntity = world.findPlayerEntity(*player);
    if (movedEntity == nullptr || movedEntity->x() != 1 ||
        movedEntity->y() != 0) {
        std::cerr << "MoveHandler did not move the logged-in PlayerEntity."
                  << std::endl;
        return 1;
    }

    Session unauthenticatedMoveSession(kInvalidSocket, 5);
    dispatcher.dispatch(movePacket, unauthenticatedMoveSession);
    movedEntity = world.findPlayerEntity(*player);
    if (movedEntity == nullptr || movedEntity->x() != 1 ||
        movedEntity->y() != 0) {
        std::cerr << "An unauthenticated Session moved a PlayerEntity."
                  << std::endl;
        return 1;
    }

    Packet malformedMovePacket;
    malformedMovePacket.protocolId =
        static_cast<std::uint16_t>(ProtocolId::Move);
    const std::string malformedMoveRequest = "dx=bad&dy=0";
    malformedMovePacket.data.assign(malformedMoveRequest.begin(),
                                     malformedMoveRequest.end());
    dispatcher.dispatch(malformedMovePacket, successfulLoginSession);
    movedEntity = world.findPlayerEntity(*player);
    if (movedEntity == nullptr || movedEntity->x() != 1 ||
        movedEntity->y() != 0) {
        std::cerr << "A malformed movement request changed the position."
                  << std::endl;
        return 1;
    }

    Session failedLoginSession(kInvalidSocket, 3);
    dispatchLogin(dispatcher, failedLoginSession,
                  "username=test&password=wrong");
    if (failedLoginSession.hasPlayer() ||
        failedLoginSession.playerId().has_value()) {
        std::cerr << "Failed login should not bind a Player." << std::endl;
        return 1;
    }

    Session malformedLoginSession(kInvalidSocket, 4);
    dispatchLogin(dispatcher, malformedLoginSession, "invalid request");
    if (malformedLoginSession.hasPlayer()) {
        std::cerr << "Malformed login should not bind a Player." << std::endl;
        return 1;
    }

    std::cout << "Dispatcher and Player binding tests passed." << std::endl;
    return 0;
}

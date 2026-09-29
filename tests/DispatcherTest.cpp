#include "Dispatcher.h"

#include "ProtocolId.h"
#include "Session.h"

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
    std::streambuf* originalOutput = std::cout.rdbuf(capturedOutput.rdbuf());
    dispatcher.dispatch(packet, session);
    std::cout.rdbuf(originalOutput);

    if (capturedOutput.str() != expectedOutput) {
        std::cerr << "Protocol " << protocolId << " produced unexpected output."
                  << " Expected: " << expectedOutput
                  << " Actual: " << capturedOutput.str();
        return false;
    }

    return true;
}

}  // namespace

int main() {
    Dispatcher dispatcher;
    Session session(kInvalidSocket, 1);

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

    std::cout << "Dispatcher tests passed." << std::endl;
    return 0;
}

#pragma once

#include "Handler.h"

class PlayerManager;
class PlayerStorage;

class LoginHandler : public Handler {
public:
    LoginHandler(PlayerManager& playerManager, PlayerStorage& playerStorage);

    void handle(const Packet& packet, Session& session) override;

private:
    PlayerManager& playerManager_;
    PlayerStorage& playerStorage_;
};

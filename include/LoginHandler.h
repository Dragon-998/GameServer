#pragma once

#include "Handler.h"

class PlayerManager;

class LoginHandler : public Handler {
public:
    explicit LoginHandler(PlayerManager& playerManager);

    void handle(const Packet& packet, Session& session) override;

private:
    PlayerManager& playerManager_;
};

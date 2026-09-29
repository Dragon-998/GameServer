#pragma once

#include "Handler.h"

class LoginHandler : public Handler {
public:
    void handle(const Packet& packet, Session& session) override;
};

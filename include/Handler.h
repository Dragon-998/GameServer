#pragma once

#include "Packet.h"

class Session;

class Handler {
public:
    virtual ~Handler() = default;

    virtual void handle(const Packet& packet, Session& session) = 0;
};

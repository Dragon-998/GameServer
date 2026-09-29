#pragma once

#include "Packet.h"

class Handler {
public:
    virtual ~Handler() = default;

    virtual void handle(const Packet& packet) = 0;
};

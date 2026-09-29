#pragma once

#include "Handler.h"

class FightHandler : public Handler {
public:
    void handle(const Packet& packet) override;
};

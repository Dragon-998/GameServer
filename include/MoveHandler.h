#pragma once

#include "Handler.h"

class MoveHandler : public Handler {
public:
    void handle(const Packet& packet) override;
};

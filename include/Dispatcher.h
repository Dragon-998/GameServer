#pragma once

#include "Handler.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

class Session;

class Dispatcher {
public:
    Dispatcher();

    void dispatch(const Packet& packet, Session& session) const;

private:
    std::unordered_map<std::uint16_t, std::unique_ptr<Handler>> handlers_;
};

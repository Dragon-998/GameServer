#pragma once

#include "Handler.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

class Dispatcher {
public:
    Dispatcher();

    void dispatch(const Packet& packet) const;

private:
    std::unordered_map<std::uint16_t, std::unique_ptr<Handler>> handlers_;
};

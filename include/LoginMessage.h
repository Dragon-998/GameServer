#pragma once

#include <string>

struct LoginRequest {
    std::string username;
    std::string password;
};

struct LoginResponse {
    bool success{false};
    std::string message;
};

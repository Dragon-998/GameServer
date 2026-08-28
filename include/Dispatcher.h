#pragma once

#include "Message.h"

#include <string>
#include <functional>
#include <unordered_map>

class Dispatcher {
public:
	using Handler = std::function<void(const Message&)>;

	void registerHandler(int cmd, Handler handler);
	void dispatch(const Message& message);

private:
	std::unordered_map<int, Handler> handlers;
};
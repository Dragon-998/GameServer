#include "Dispatcher.h"

#include <iostream>

void Dispatcher::registerHandler(int cmd, Handler handler)
{
	handlers[cmd] = handler;
}

void Dispatcher::dispatch(const Message& message)
{
	auto it = handlers.find(message.cmd);
	
	if (it != handlers.end()) {
		it->second(message);
	}
	else {
		std::cout << "unknown cmd" << std::endl;
	}
}

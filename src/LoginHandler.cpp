#include "LoginHandler.h"

#include <iostream>

void LoginHandler::Handler(const Message& message)
{
	std::cout << "Íæ¼ÒµÇÂ¼£º" << message.data << std::endl;
}

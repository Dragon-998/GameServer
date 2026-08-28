// GameServer.cpp: 定义应用程序的入口点。
//

#include "GameServer.h"
#include "Dispatcher.h"
#include "Message.h"
#include "LoginHandler.h"
#include "Player.h"

#include <unordered_map>
#include <memory>

int main()
{
	std::unordered_map<int, std::shared_ptr<Player>> players;
	auto player = std::make_shared<Player>(10001, "mei");
	players[player->getId()] = player;

	Dispatcher dispatcher;
	LoginHandler loginHandler;
	dispatcher.registerHandler(1001, [&loginHandler](const Message& message) {
		loginHandler.Handler(message);
	});
	dispatcher.registerHandler(1002, [](const Message& message) {
		std::cout << "收到消息：" << message.data << std::endl;
	});

	Message loginMessage{ 1001,"player1" };
	Message chatMessage{ 1002,"Hello World!" };

	dispatcher.dispatch(loginMessage);
	dispatcher.dispatch(chatMessage);

	return 0;
}

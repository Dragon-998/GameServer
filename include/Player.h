#pragma once

#include <string>

class Player {
public:
	Player(int id, const std::string& name);
	int getId() const;
	const std::string& getName() const;

private:
	int playerId;
	std::string PlayerName;
};
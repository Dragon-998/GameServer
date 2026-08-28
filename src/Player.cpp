#include "Player.h"

Player::Player(int id, const std::string& name)
	:playerId(id)
	, PlayerName(name)
{

}

int Player::getId() const
{
	return this->playerId;
}

const std::string& Player::getName() const
{
	return this->PlayerName;
}

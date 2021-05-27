#include "Player.h"

Player::Player(std::string name)
{
   this->name = name;
   this->score = 0;
   hand = new LinkedList();
}

Player::Player(std::string name, int score)
{
   this->name = name;
   this->score = score;
   hand = nullptr;
}

Player::Player(std::string name, int score, LinkedList *hand)
{
   this->name = name;
   this->score = score;
   this->hand = hand;
}

Player::~Player()
{
   delete hand;
   hand = nullptr;
}

bool Player::operator<(const Player &player) const
{
   return (player.score < score);
}

// Update the name of a player with a string input
void Player::setName(std::string newName)
{
   this->name = newName;
}

// Return a string of the players name
std::string Player::getName()
{
   return this->name;
}

// Increase the players score by an int input
void Player::addScore(int scoreToAdd)
{
   this->score += scoreToAdd;
}

// Return the score of the player as an int
int Player::getScore()
{
   return this->score;
}

// Returns the players hand
LinkedList *Player::getHand()
{
   return hand;
}

void Player::setNumber(int playerNumber)
{
   this->playerNumber = playerNumber;
}

int Player::getNumber()
{
   return playerNumber;
}
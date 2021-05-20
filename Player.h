#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>
#include <iostream>

#include "LinkedList.h"

class Player{
public:
   Player(std::string name);
   Player(std::string name, int score, LinkedList* hand);
   ~Player();

   std::string getName();
   
   bool operator > (const Player& player) const;
   int getScore();

   int getNumber();
   void setNumber(int playerNumber);
   void setName(std::string newName);
   void printHand();
   void addTile(Tile* newTile);
   void addScore(int scoreToAdd);

   Tile* getTile();

   LinkedList* getHand();

   std::string handToString();

   

private:
   std::string name;
   int score;
   LinkedList* hand;

   int playerNumber;
};

# endif //PLAYER_H

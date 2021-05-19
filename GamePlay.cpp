#include <fstream>

#include "GamePlay.h"

GamePlay::GamePlay()
{
   
}

GamePlay::~GamePlay()
{
   delete theBoard;
   theBoard = nullptr;
   delete menu;
   menu = nullptr;
}


void GamePlay::setPlayer(std::vector<Player* > thePlayers)
{
   this->thePlayers = thePlayers;
}

void GamePlay::setBoard(Board* board)
{
   theBoard = board;
}

Board* GamePlay::getBoard()
{
   return theBoard;
}

void GamePlay::setMenu(Menu* menu)
{
   this->menu = menu;
}

Menu* GamePlay::getMenu()
{
   return menu;
}

std::vector<Player* > GamePlay::getPlayers()
{
   return thePlayers;
}
// One players turn
// Will loop until successful move or quit
bool GamePlay::playerMove(int playerTurn)
{
   bool tilePlaced = false;
   bool tileReplaced = false;
   bool gameSaved = false;
   bool triedToSaveGame = false;

   Player* player;
   Player* playerTwo;

   if(player1->getNumber() == playerTurn)
   {
      player = player1;
      playerTwo = player2;
   }
   else
   {
      player = player2;
      playerTwo = player1;
   }
   


   std::cout << player->getName() << ". Your hand is: " << std::endl;
   std::cout << player->getHand()->llToString() << std::endl;
   std::cout << std::endl;
   std::cout << "What would you like to play and where?" << std::endl;

   while (!tileReplaced && !tilePlaced && !menu->getQuit())
   {

      std::vector<std::string> wordsIn = menu->takeLineInput(' ');
      triedToSaveGame = false;
      if (!menu->getQuit())
      {
         if (wordsIn.size() == 4 && wordsIn[0] == "place" && wordsIn[2] == "at")
         {
            tilePlaced = placeTile(wordsIn, player);
            if(player->getHand()->getSize() == 0)
            {
               std::cout << std::endl;
               theBoard->toString();
            }
         }
         else if (wordsIn.size() == 2 && wordsIn[0] == "replace")
         {
            tileReplaced = replaceTile(wordsIn, player);
         }
         else if(wordsIn.size() == 2 && wordsIn[0] == "save")
         {
            gameSaved = saveGame(wordsIn, player, playerTwo);
            std::cout << "Game successfully saved" <<std::endl;
            triedToSaveGame = true;
         }
         else
         {
            std::cout << "Incorrect Input!" << std::endl;
            std::cout << "Please input again" << std::endl;
         }
         if(triedToSaveGame && !gameSaved)
         {
            std::cout << "Failed to save!" << std::endl;
         }
      }
   }
  return !menu->getQuit();
}

// Takes the tile inputted and determines if it is in the players hand
bool GamePlay::tileInputtedIsOkay(std::string tileString, Player *player)
{
   bool isOkay = false;
   char colour = tileString[0];
   int shape = menu->charToInt(tileString[1]);

   if (tileString.size() == 2 && shape >= CIRCLE && shape <= CLOVER)
   {
      if (colour == ORANGE || RED || YELLOW || GREEN || BLUE || PURPLE)
      {

         Tile *tile = new Tile(colour, shape);
         if (player->getHand()->isInLinkedList(tile))
         {
            isOkay = true;
         }
         delete tile;
      }
   }
   return isOkay;
}

bool GamePlay::legalMove(Player* player)
{
   bool check = false;
   if(!theBoard->checkEmpty())
   {

      for (int i=0; i < theBoard->getRows() && check == false; i++)
      {
         for (int j=0; j < theBoard->getCols() && check == false; j++)
         {
            for (int k=0; k < player->getHand()->getSize() && check == false; k++)
            {
               Location location(i,j);
               Tile* tile = player->getHand()->get(k);
               check = tileFit(tile, location);
            }
         }
      }
   }
   else
   {
      check = true;
   }
   return check;
}

// Changes an input string to a real tile
// Only called if the tile is a possible tile
Tile* GamePlay::turnInputToTile(std::string tiledata)
{
   char colour = tiledata[0];
   int shape = menu->charToInt(tiledata[1]);
   Tile *tile = new Tile(colour, shape);
   return tile;
}

// Converts an input location string to a location
Location GamePlay::convertInputLoc(std::string inputLocation)
{
   Location location;
   location.row = (int)inputLocation[0] - 65;

   // Depending on whether inputted string is 2 or 3 digits
   if (inputLocation.size() == 3)
   {
      int tens = menu->charToInt(inputLocation[1]);
      int ones = menu->charToInt(inputLocation[2]) - INDEXING;
      location.col = (10 * tens) + (ones);
   }
   else
   {
      location.col = menu->charToInt(inputLocation[1] ) - INDEXING;
   }
   return location;
}

// Places a tile in a location if it is an acceptable location
bool GamePlay::placeTile(std::vector<std::string> wordsIn, Player *player)
{
   Tile *checkTile = nullptr;

   bool isSpotTaken = true;
   bool acceptableTile = false;
   bool moveMade = false;
   bool locExists = false;
   bool acceptableLoc = false;


   acceptableTile = tileInputtedIsOkay(wordsIn[1], player);
   checkTile = new Tile(wordsIn[1][0], menu->charToInt(wordsIn[1][1]));


   Location toPlace = convertInputLoc(wordsIn[3]);
   locExists = theBoard->isOnBoard(toPlace);

   // takes correct location and looks if it is an empty position on the Board
   if (locExists)
   {
      isSpotTaken = !theBoard->emptyLocation(toPlace);
      acceptableLoc = tileFit(checkTile, toPlace);
   }


   // if player input were correct place the tile, put new tile in players hand
   if (!isSpotTaken && acceptableTile && locExists && acceptableLoc)
   {
      int tileIndex = player->getHand()->findSpecificTile(checkTile);
      player->getHand()->removeAt(tileIndex);
      theBoard->placeTile(checkTile, toPlace);

      // Hand new tile to the player SHOULD BE A METHOD
      HandPlayerTile(player);
      player->addScore(score(toPlace));
      moveMade = true;   
   }
   else
   {
      std::cout << std::endl;
      std::cout << "Tile cannot be placed there!" << std::endl;
      delete checkTile;
   }

   return moveMade;
}

// Checks if a tile can legally be placed in a location
bool GamePlay::tileFit(Tile* tile, Location location)
{
   bool check = true;

   if (!theBoard->checkEmpty())
   {
      if(!checkBothSides(UP, DOWN, location, tile) || !checkBothSides(RIGHT, LEFT, location, tile))
      {
         check = false;
      }
      if (!checkIfNextToTiles(location))
      {
         check = false;
      }
   }

   return check;
}

// Replaces a tile in a players hand if legal to do
bool GamePlay::replaceTile(std::vector<std::string> wordsIn, Player *player)
{
   bool rtnReplaced = false;

   bool tileInputInHand = tileInputtedIsOkay(wordsIn[1], player);

   if (tileInputInHand && theBoard->getBag()->getSize() != 0)
   {

      Tile *checkTile = turnInputToTile(wordsIn[1]);
      int tileIndex = player->getHand()->findSpecificTile(checkTile);
      // Tile *playersTile = player->getHand()->get(tileIndex);
      player->getHand()->removeAt(tileIndex);
      theBoard->getBag()->addBack(checkTile);

      HandPlayerTile(player);

      rtnReplaced = true;
      // delete checkTile;
   }
   else if(theBoard->getBag()->getSize() == 0)
   {
      std::cout << "The bag is empty!" <<std::endl;
   }
   else
   {
      std::cout << "That tile is not in your hand!" << std::endl;
   }


   return rtnReplaced;
}

// returns false if tile shouldnt be placed because of tiles around it
bool GamePlay::checkBothSides(int direction1, int direction2, Location location, Tile* tile)
{
   Location checkLocation;
   bool check = true;
   checkLocation.row = location.getNextRow(direction1);
   checkLocation.col = location.getNextCol(direction1);

   std::vector<Tile*>* tileInLine = new std::vector<Tile*>();
   tileInLine->push_back(tile);
   checkDirection(direction1, location, tileInLine);
   checkDirection(direction2, location, tileInLine);

   check = !compareTiles(tileInLine);

   for (long unsigned int i = 0; i< tileInLine->size(); i++)
   {
      tileInLine->pop_back();
   }
   delete tileInLine;
   
   return check;
}

// Return false if not next to any tiles
bool GamePlay::checkIfNextToTiles(Location location)
{
   Location checkLocation;
   bool check = true;
   std::vector<Tile*>* tileInLine = new std::vector<Tile*>();
   for(int i =UP; i<= LEFT; i++)
   {
      checkLocation.row = location.getNextRow(i);
      checkDirection(i, location, tileInLine);
   }
   if (tileInLine->size() == 0)
   {
      check = false;
   }
   delete tileInLine;
   return check;
}

// Adds all tiles in one direction from a location into a vector
void GamePlay::checkDirection(int direction1, Location location, std::vector<Tile*>* tileInLine)
{
   Location checkLocation;
   checkLocation.row = location.getNextRow(direction1);
   checkLocation.col = location.getNextCol(direction1);

   bool empty = false;
   while(!empty)
   {
      if (theBoard->isOnBoard(checkLocation))
      {
         if(!theBoard->emptyLocation(checkLocation))
         {
            theBoard->getTile(checkLocation);
            tileInLine->push_back(theBoard->getTile(checkLocation));

            checkLocation.row = checkLocation.getNextRow( direction1);
            checkLocation.col = checkLocation.getNextCol( direction1);
         }
         else 
         {
            empty = true;
         }
      }
      else
      {
         empty = true;
      }
   }
}

// Returns true if two tiles in array are the same
bool GamePlay::compareTiles(std::vector<Tile*>* tileInLine)
{
   bool match = false;
   // Shape nextShape;
   // Colour nextColour;


   Shape shape;
   Colour colour;

   bool shapeCheck = true;
   bool colourCheck = true;

   for(long unsigned int i = 0; i < tileInLine->size() - 1; i++)
   {
      shape = tileInLine->at(i)->getShape();
      colour = tileInLine->at(i)->getColour();

      for (long unsigned int j = i + 1; j < tileInLine->size(); j++)
      {
         if(tileInLine->at(j)->getShape() != shape)
         {
            shapeCheck = false;
         }
         if(tileInLine->at(j)->getColour() != colour)
         {
            colourCheck = false;
         }
         if(tileInLine->at(j)->getShape() == shape && tileInLine->at(j)->getColour() == colour)
         {
            match = true;
         }
      }
   }

   if(!colourCheck && !shapeCheck)
   {
      match = true;
   }

   return match;
}

// Hands the first tile from the bag to the player
void GamePlay::HandPlayerTile(Player* player)
{
   if (theBoard->getBag()->getSize() != 0)
   {
      Tile *tmpTile = theBoard->getBag()->getFront();
      theBoard->getBag()->removeFront();
      player->getHand()->addBack(tmpTile);
   }
}

// Saves a game into a file
bool GamePlay::saveGame(std::vector<std::string> wordsIn, Player *player, Player* player2)
{

   bool saveCheck = false;
   std::string fileExtension = ".save";
   std::string fileName = wordsIn[1];

   fileName = fileName.append(fileExtension);
   // std::cout << fileName <<std::endl;
   
   std::ofstream MyFile(fileName);
   if(!MyFile.fail())
   {
      if(player->getNumber() == 1)
      {
         MyFile << player->getName() << std::endl;
         MyFile << player->getScore() << std::endl;
         MyFile << player->getHand()->llToString() << std::endl;
         MyFile << player2->getName() << std::endl;
         MyFile << player2->getScore() << std::endl;
         MyFile << player2->getHand()->llToString() << std::endl;
      }
      else
      {
         MyFile << player2->getName() << std::endl;
         MyFile << player2->getScore() << std::endl;
         MyFile << player2->getHand()->llToString() << std::endl;
         MyFile << player->getName() << std::endl;
         MyFile << player->getScore() << std::endl;
         MyFile << player->getHand()->llToString() << std::endl;

      }

      MyFile << NO_OF_ROWS << ",";
      MyFile << NO_OF_COLS <<std::endl;

      MyFile << theBoard->saveBoard() <<std::endl;
      MyFile << theBoard->getBag()->llToString() <<std::endl;
      MyFile << player->getName() << std::endl;

      saveCheck = true;
      
   }
   MyFile.close();

   
   return saveCheck;
}

// Determines the score of a move
int GamePlay::score(Location location)
{
   int score  = 0;
   Location nextLocation;

   
   for (int direction = UP; direction <= RIGHT; direction++)
   {
      
      nextLocation.row = location.row;
      nextLocation.col = location.col;
      
      int counter = 0;
      counter += scoreDirection(direction, nextLocation);
      nextLocation.row = location.row;
      nextLocation.col = location.col;
      if (direction == UP)
      {
         counter +=scoreDirection(DOWN, nextLocation);
      }
      nextLocation.row = location.row;
      nextLocation.col = location.col;
      if (direction == RIGHT)
      {
         counter += scoreDirection(LEFT, nextLocation);
      }
      if (counter != 0)
      {
         counter ++;
      }
      if(counter == 6)
      {
         std::cout << std::endl;
         std::cout << "QWIRKLE!!!" <<std::endl;

         counter += 6;
      }

      score += counter;

   }

   if (score == 0)
   {
      score ++;
   }
   
   return score;
}

// Determines the score in a single direction from a tile
int GamePlay::scoreDirection(int direction, Location location)
{
   bool Empty = false;
   int score = 0;
   while(!Empty)
   {
      location.col = location.getNextCol(direction);
      location.row = location.getNextRow(direction);
      if(theBoard->isOnBoard(location))
      {
         Empty = theBoard->emptyLocation(location);
         if (!Empty)
         {
            score++;
         }
      }
      else
      {
         Empty = true;
      }
   }

   return score;
}

void GamePlay::handOutBonusPoints()
{
   if(player1->getHand()->getSize() == 0)
   {
      player1->addScore(6);
   }
   else if (player2->getHand()->getSize() == 0)
   {
      player2->addScore(6);
   }
}
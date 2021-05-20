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
void GamePlay::playerMove(int playerTurn)
{
   bool tileReplaced = false;
   bool gameSaved = false;
   bool triedToSaveGame = false;
   bool endTurn = false;

   std::vector<Move> theMoves;
   std::vector<std::string> wordsIn;
   while (!tileReplaced && !endTurn && !menu->getQuit())
   {
      wordsIn.empty();
      if(theMoves.size() == 0)
      {
         std::cout << thePlayers.at(playerTurn)->getName() << ". Your hand is: " << std::endl;
         std::cout << thePlayers.at(playerTurn)->getHand()->llToString() << std::endl;
         std::cout << std::endl;
         
         std::cout << "What move would you like to do?" << std::endl;
         wordsIn = menu->takeLineInput(' ');
      }
      else
      {
         theBoard->toString();
         std::cout << thePlayers.at(playerTurn)->getName() << ". Your hand is: " << std::endl;
         std::cout << thePlayers.at(playerTurn)->getHand()->llToString() << std::endl;
         std::cout << "Would you like to place again?" << std::endl;
         std::cout << "Typing 'end' will end your turn" << std::endl;
         wordsIn = menu->takeLineInput(' ');
      }

      triedToSaveGame = false;

      if (!menu->getQuit())
      {  
         if (wordsIn.size() == 4 && wordsIn[0] == "place" && wordsIn[2] == "at")
         {
            theMoves = place(wordsIn, playerTurn, theMoves);
         }
         else if (wordsIn.size() == 2 && wordsIn[0] == "replace" && theMoves.size() == 0)
         {
            tileReplaced = replaceTile(wordsIn, playerTurn);
            endTurn = true;
         }
         else if(wordsIn.size() == 2 && wordsIn[0] == "save")
         {
            gameSaved = saveGame(wordsIn, playerTurn);
            std::cout << "Game successfully saved" <<std::endl;
            triedToSaveGame = true;
         }
         else if(theMoves.size() != 0 && wordsIn.size() == 1 && wordsIn[0] == "end")
         {
            endTurn = true;
         }
         else
         {
            std::cout << "Incorrect Input!" << std::endl;
            std::cout << "Please input again" << std::endl;
         }

         if(theMoves.size() == 6)
         {
            std::cout << "Your have placed all your tiles" << std::endl;
            std::cout << "Your turn is over" << std::endl;
            endTurn = true;
         }

         if(triedToSaveGame && !gameSaved)
         {
            std::cout << "Failed to save!" << std::endl;
         }
      }
   }
   if(!menu->getQuit())
   {
      thePlayers.at(playerTurn)->addScore(score(theMoves));
   }
  
}


std::vector<Move> GamePlay::place(std::vector<std::string> wordsIn, int playerTurn, std::vector<Move> theMoves)
{
   std::vector<Move> moves = theMoves;
   if (tileInputtedIsOkay(wordsIn[1], playerTurn))
   {
      Tile* tile = turnInputToTile(wordsIn[1]);
      Location location = convertInputLoc(wordsIn[3]);
      
      if(checkNextTo(theMoves, tile, location) && placeTile(wordsIn, playerTurn))
      {
         Move move(location, tile);
         moves.push_back(move);
      }
   }
   return moves;
}

// Takes the tile inputted and determines if it is in the players hand
bool GamePlay::tileInputtedIsOkay(std::string tileString, int playerTurn)
{
   bool isOkay = false;
   char colour = tileString[0];
   int shape = menu->charToInt(tileString[1]);

   if (tileString.size() == 2 && shape >= CIRCLE && shape <= CLOVER)
   {
      if (colour == ORANGE || RED || YELLOW || GREEN || BLUE || PURPLE)
      {

         Tile *tile = new Tile(colour, shape);
         if (thePlayers.at(playerTurn)->getHand()->isInLinkedList(tile))
         {
            isOkay = true;
         }
         delete tile;
      }
   }
   return isOkay;
}

bool GamePlay::inputtedLocIsOkay(std::string location)
{
   bool okay = false;
   if(location.size() == 2 || location.size() == 3)
   {
      if(location[0] >= 'A' && location[0] <= 'Z')
      {
         if(location.size() == 2 && location[1] >= '0' && location[1] <= '9')
         {
            okay = true;
         }
         else if(location.size() == 3 && location[1] >= '0' && location[1] <= '9'
         && location[2] >= '0' && location[2] <=9)
         {
            okay = true;
         }
      }
   }
   return okay;
}


bool GamePlay::legalMove(int playerTurn)
{
   bool check = false;
   if(!theBoard->checkEmpty())
   {
      for (int i=0; i < theBoard->getRows() && check == false; i++)
      {
         for (int j=0; j < theBoard->getCols() && check == false; j++)
         {
            for (int k=0; k < thePlayers.at(playerTurn)->getHand()->getSize() && check == false; k++)
            {
               Location location(i,j);
               Tile* tile = thePlayers.at(playerTurn)->getHand()->get(k);
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
bool GamePlay::placeTile(std::vector<std::string> wordsIn, int playerTurn)
{
   Tile *checkTile = nullptr;

   bool isSpotTaken = true;
   bool acceptableTile = false;
   bool moveMade = false;
   bool locExists = false;
   bool acceptableLoc = false;


   acceptableTile = tileInputtedIsOkay(wordsIn[1], playerTurn);
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
      int tileIndex = thePlayers.at(playerTurn)->getHand()->findSpecificTile(checkTile);
      thePlayers.at(playerTurn)->getHand()->removeAt(tileIndex);
      theBoard->placeTile(checkTile, toPlace);

      // Hand new tile to the player SHOULD BE A METHOD
      // HandPlayerTile(playerTurn);
   // thePlayers.at(playerTurn)->addScore(score(toPlace));
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

bool GamePlay::checkNextTo(std::vector<Move> theMoves, Tile* tile, Location location)
{
   bool match = false;

   bool colMatch = true;
   bool rowMatch = true;
   Location nextLoc;
   if(theMoves.size() != 0)
   {
      for(int i = 0; i< theMoves.size(); i++)
      {
         if(location.col != theMoves.at(i).location.col)
         {
            colMatch = false;
         }
         if(location.row != theMoves.at(i).location.row)
         {
            rowMatch = false;
         }

         Location theMoveLoc = theMoves.at(i).location;
         
         for(int i = UP; i<= LEFT; i++)
         {
            nextLoc.col = location.getNextCol(i);
            nextLoc.row = location.getNextRow(i);

            if(theMoveLoc.col == nextLoc.col && theMoveLoc.row == nextLoc.row)
            {
               match = true;
            }
         }
      }
      if(!colMatch && !rowMatch)
      {
         match = false;
      }
   }
   else
   {
      match = true;
   }

   return match; 
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
bool GamePlay::replaceTile(std::vector<std::string> wordsIn, int playerTurn)
{
   bool rtnReplaced = false;

   bool tileInputInHand = tileInputtedIsOkay(wordsIn[1], playerTurn);

   if (tileInputInHand && theBoard->getBag()->getSize() != 0)
   {

      Tile *checkTile = turnInputToTile(wordsIn[1]);
      int tileIndex = thePlayers.at(playerTurn)->getHand()->findSpecificTile(checkTile);
      
      thePlayers.at(playerTurn)->getHand()->removeAt(tileIndex);
      theBoard->getBag()->addBack(checkTile);

      HandPlayerTile(playerTurn);

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
void GamePlay::HandPlayerTile(int playerTurn)
{
   if (theBoard->getBag()->getSize() != 0)
   {
      Tile *tmpTile = theBoard->getBag()->getFront();
      theBoard->getBag()->removeFront();
      thePlayers.at(playerTurn)->getHand()->addBack(tmpTile);
   }
}

// Saves a game into a file
bool GamePlay::saveGame(std::vector<std::string> wordsIn, int playersTurn)
{

   bool saveCheck = false;
   std::string fileExtension = ".save";
   std::string fileName = wordsIn[1];

   fileName = fileName.append(fileExtension);
   
   std::ofstream MyFile(fileName);
   if(!MyFile.fail())
   {
      for(int i =0; i< thePlayers.size(); i++)
      {
         MyFile << thePlayers.at(i)->getName() << std::endl;
         MyFile << thePlayers.at(i)->getScore() << std::endl;
         MyFile << thePlayers.at(i)->getHand()->llToString() << std::endl;
      }

      MyFile << NO_OF_ROWS << ",";
      MyFile << NO_OF_COLS <<std::endl;

      MyFile << theBoard->saveBoard() <<std::endl;
      MyFile << theBoard->getBag()->llToString() <<std::endl;
      MyFile << thePlayers.at(playersTurn)->getName() << std::endl;

      saveCheck = true;
      
   }
   MyFile.close();

   
   return saveCheck;
}

// Determines the score of a move
int GamePlay::score(std::vector<Move> theMoves)
{
   int score  = 0;
   Location nextLocation;
   bool inMoves = false;

   score += theMoves.size();
   if(theMoves.size() == 6)
   {
      std::cout << std::endl;
      std::cout << "QWIRKLE!!!" <<std::endl;
   }
   for(int i = 0; i < theMoves.size(); i++)
   {
      for (int direction = UP; direction <= RIGHT; direction++)
      {
         int counter = 0;
         nextLocation.row = theMoves.at(i).location.getNextRow(direction);
         nextLocation.col = theMoves.at(i).location.getNextCol(direction);
         
         for(int j = 0; j < theMoves.size(); j++)
         {
            if(nextLocation.row == theMoves.at(j).location.row
            && nextLocation.col == theMoves.at(j).location.col)
            {
               inMoves = true;
            }
         }
         if(!inMoves)
         {
            counter += scoreDirection(direction, nextLocation);
         }

         int otherDirection;
         
         if (direction == UP)
         {
            otherDirection = DOWN;
         }
         else if (direction == RIGHT)
         {
            otherDirection = LEFT;
         }

         for(int j = 0; j < theMoves.size(); j++)
         {
            nextLocation.row = theMoves.at(i).location.getNextRow(otherDirection);
            nextLocation.col = theMoves.at(i).location.getNextCol(otherDirection);

            if(nextLocation.row == theMoves.at(i).location.row
            && nextLocation.col == theMoves.at(i).location.col)
            {
               inMoves = true;
            }
         }
         if(!inMoves)
         {
            counter += scoreDirection(otherDirection, theMoves.at(i).location);
         }

         if(counter == 6)
         {
            std::cout << std::endl;
            std::cout << "QWIRKLE!!!" <<std::endl;

            counter += 6;
         }

         score += counter;

      }
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
   for(int i = 0; i< thePlayers.size(); i++)
   {
      if(thePlayers.at(i)->getHand()->getSize() == 0)
      {
         thePlayers.at(i)->addScore(6);
      }
   }
}
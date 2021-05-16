#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

#include "GamePlay.h"

#define EXIT_SUCCESS 0

void menu();

void credits();
void NewGame(GamePlay *gameTime);
void LoadGame(GamePlay* gameTime);
std::vector<Tile *> initialiseTileBag();
void handingTilesToPlayers(Player *player1, Player *player2, Board *theBoard);
void playingTheGame(Player *player1, Player *player2, GamePlay *gameTime);
Player* loadInPlayer(std::ifstream& saveFile, Menu* menu);
Board* loadInBoard(std::ifstream& saveFile, Menu* menu);
std::vector<std::string> splitString(std::string string, std::string delim);
void onePlayerTurn(Player* currentPlayer, Player* otherPlayer, GamePlay* gameTime);

int main(void)
{
   // bool quit = false;
   Menu *theMenu = new Menu();
   GamePlay *gameTime = new GamePlay();
   gameTime->setMenu(theMenu);
   std::cout << "Welcome to Qwirkle!" << std::endl;
   std::cout << "-------------------" << std::endl;

   std::vector<std::string> userString;
   std::string userInput = "";

   while (!theMenu->getQuit())
   {
      theMenu->printMenu();
      userString = theMenu->takeLineInput(' ');
      if(!theMenu->getQuit())
      {
         if (userString.size() == 1)
         {
            if (userString.size() == 1)
            {
               // set the one word to the menu choice
               userInput = userString[0];
            }

            if (userInput == "1")
            {
               NewGame(gameTime);
            }
            else if (userInput == "2")
            {
               LoadGame(gameTime);
            }
            else if (userInput == "3")
            {
               theMenu->printCredits();
            }
            else if (userInput == "4")
            {
               // quit = true;
            }
            else
            {
               std::cout << "Invalid Input!" << std::endl;
            }
         }
      }
   }
   delete gameTime;

   std::cout << "GoodBye!" << std::endl;
   return EXIT_SUCCESS;
}


// Starting a new game, setting up everything
void NewGame(GamePlay* gameTime)
{
   std::string name1 = "";
   std::string name2 = "";

   std::cout << std::endl;
   std::cout << "Starting a New Game" << std::endl
             << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   name1 = gameTime->getMenu()->getName();

   if (!gameTime->getMenu()->getQuit())
   {
      Player *player1 = new Player(name1);
      player1->setNumber(1);
      std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
      std::cout << ">";
      name2 = gameTime->getMenu()->getName();

      if (!gameTime->getMenu()->getQuit())
      {
         Player *player2 = new Player(name2);
         player2->setNumber(2);

         Board *board = new Board();

         std::vector<Tile *> tPtrs = initialiseTileBag();
         LinkedList* bag = new LinkedList();
         board->setBag(bag);

         for (Tile *tile : tPtrs)
         {
            board->getBag()->addFront(tile);
         }
         
         handingTilesToPlayers(player1, player2, board);
         gameTime->setPlayer(player1);
         gameTime->setPlayer(player2);
         gameTime->setBoard(board);
         playingTheGame(player1, player2, gameTime);
      }
   }
}

// Places tiles into a vector and returns it
std::vector<Tile *> initialiseTileBag()
{

   Colour tileColours[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
   Shape tileShapes[] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

   std::vector<Tile *> orderedTiles;

   for (int i = 0; i < NO_OF_TILESETS; i++)
   {
      for (Colour colour : tileColours)
      {
         for (Shape shape : tileShapes)
         {
            Tile *tile = new Tile(colour, shape);
            orderedTiles.push_back(tile);
         }
      }
   }
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   auto rng = std::default_random_engine {seed};
   std::shuffle(std::begin(orderedTiles), std::end(orderedTiles), rng);

   return orderedTiles;
}

// Places tiles from the bag into players hand
void handingTilesToPlayers(Player *player1, Player *player2, Board *theBoard)
{

   if (theBoard->getBag()->getSize() >= 12)
   {
      Tile *theTile;
      for (int i = 0; i < 6; i++)
      {
         theTile = theBoard->getBag()->getFront();
         player1->getHand()->addFront(theTile);
         theBoard->getBag()->removeFront();
         theTile = theBoard->getBag()->getFront();
         player2->getHand()->addFront(theTile);
         theBoard->getBag()->removeFront();
      }
   }
}

// Playing the game
void playingTheGame(Player *player1, Player *player2, GamePlay *gameTime)
{
   bool gameover = false;
   while (player1->getHand()->getSize() != 0 && player2->getHand()->getSize() != 0 && !gameTime->getMenu()->getQuit() && gameover == false)
   {
      if(gameTime->getBoard()->getBag()->getSize() != 0 || gameTime->legalMove(player1))
      {
         onePlayerTurn(player1, player2, gameTime);
      }
      else
      {
         gameover = true;

         std::cout << std::endl;
         std::cout << "You have no legal moves" << std::endl;
         std::cout << "Therefore the game is over!" << std::endl << std::endl;
      }

      if (!gameTime->getMenu()->getQuit() && player1->getHand()->getSize() != 0 && gameover == false)
      {
         if(gameTime->getBoard()->getBag()->getSize() != 0 || gameTime->legalMove(player2))
         {
            onePlayerTurn(player2, player1, gameTime);
         }
         else
         {
            gameover = true;
            std::cout << std::endl;
            std::cout << "You have no legal moves" << std::endl;
            std::cout << "Therefore the game is over!" << std::endl << std::endl;
         }
      }
   }
   
   if (!gameTime->getMenu()->getQuit())
   {
      gameTime->handOutBonusPoints();
      std::string winnerName;
      int winnerScore;
      std::string loserName;
      int loserScore;
      if (player1->getScore() > player2->getScore())
      {
         winnerName = player1->getName();
         winnerScore = player1->getScore();
         loserName = player2->getName();
         loserScore = player2->getScore();

         std::cout << "Congratulations " << winnerName << " won with a score of: " << winnerScore << std::endl;
         std::cout << "Better luck next time " << loserName << " lost by: " << (winnerScore - loserScore) << std::endl;
      }
      else if (player2->getScore() > player1->getScore())
      {
         winnerName = player2->getName();
         winnerScore = player2->getScore();
         loserName = player1->getName();
         loserScore = player1->getScore();

         std::cout << "Congratulations " << winnerName << " won with a score of: " << winnerScore << std::endl;
         std::cout << "Better luck next time " << loserName << " lost by: " << winnerScore - loserScore << std::endl;

      }
      else
      {
         std::cout << "... It's tie! You both scored: " << player1->getScore() << std::endl;
      }
      gameTime->getMenu()->setQuit(true);
   }
}

// Start of a players turn
void onePlayerTurn(Player* currentPlayer, Player* otherPlayer, GamePlay* gameTime)
{
   if(currentPlayer->getNumber() == 1)
   {
      std::cout << currentPlayer->getName() << " it is your turn" << std::endl;
      std::cout << currentPlayer->getName() << "'s score: " << currentPlayer->getScore() << std::endl;
      std::cout << otherPlayer->getName() << "'s score: " << otherPlayer->getScore() << std::endl;
   }
   else
   {
      std::cout << currentPlayer->getName() << " it is your turn" << std::endl;
      std::cout << otherPlayer->getName() << "'s score: " << otherPlayer->getScore() << std::endl;
      std::cout << currentPlayer->getName() << "'s score: " << currentPlayer->getScore() << std::endl;
   }
   gameTime->getBoard()->toString();

   gameTime->playerMove(currentPlayer->getNumber());
}

//Loading saved game details from a save file
void LoadGame(GamePlay* play)
{

   std::vector<std::string> filename;
   std::string file;
   Player* player2;
   Player* player1;
   Board* theBoard;

    std::cout << "Enter the filename from which to load a game" << std::endl;
    filename = play->getMenu()->takeLineInput(' ');
    if(filename.size() == 1 && !play->getMenu()->getQuit())
    {
      file = filename[0];
      file += ".save";
      std::ifstream saveFile(file);

      if(saveFile.is_open())
      {

         player1 = loadInPlayer(saveFile, play->getMenu());
         player1->setNumber(1);
         player2 = loadInPlayer(saveFile, play->getMenu());
         player2->setNumber(2);
         theBoard = loadInBoard(saveFile, play->getMenu());

         std::string playerTurn = "";

         std::getline(saveFile, playerTurn);
      
         play->setPlayer(player1);
         play->setPlayer(player2);
         play->setBoard(theBoard);

         if(player1->getName() == playerTurn)
         {
            playingTheGame(player1, player2, play);
         }
         else
         {
            playingTheGame(player2, player1, play);
         }
      }
      else
      {
         std::cout << std::endl;
         std::cout << "File does not exist!" << std::endl;
      }
   }
}
//Loads in player
Player* loadInPlayer(std::ifstream& saveFile, Menu* menu)
{
//Read data for player one and two
   std::string playerName = "";
   std::string playerScore= "";
   std::string playerHand= "";
   std::vector<std::string> playerHandVector;
   int player1score= 0;
   int hundreds= 0;
   int tens = 0;
   int ones = 0;

   //Read data for player one
   if (saveFile.is_open())
   {
      std::getline(saveFile, playerName);
      std::getline(saveFile, playerScore);
      std::getline(saveFile, playerHand);
   }

   if (playerScore.size() == 3)
   {
      hundreds = menu->charToInt(playerScore[0]);
      tens = menu->charToInt(playerScore[1]);
      ones = menu->charToInt(playerScore[2]);
      player1score = (100*hundreds)+(10 * tens) + (ones);
   }
   else if(playerScore.size() == 2)
   {
      tens = menu->charToInt(playerScore[0]);
      ones = menu->charToInt(playerScore[1]);
      player1score = (10 * tens) + (ones);
   }
   else
   {
      player1score = ones;
   }
   
   playerHandVector = splitString(playerHand, ",");

   LinkedList* player1Hand = new LinkedList();
   for (unsigned int i =0; i < playerHandVector.size(); i++)
   {

      Colour colour = playerHandVector[i][0];
      Shape shape = menu->charToInt(playerHandVector[i][1]);
      Tile* newTile = new Tile(colour, shape);
      player1Hand->addBack(newTile);
      // delete newTile;
      
   }

   Player* playerOne = new Player(playerName, player1score, player1Hand);
   // delete player1Hand;
   return playerOne;
}
//Loads in the tiles placed on the board
Board* loadInBoard(std::ifstream& saveFile, Menu* menu)
{
   std::vector<std::string> boardDimentions;
   std::vector<std::string> locationsW;
   std::vector<std::string> bagTiles;

   std::string locations = "";
   std::string theBagString = "";
   Board* theBoard = new Board();

   LinkedList* theBag = new LinkedList();
   int row = 0;
   int col = 0;
   std::string dimentions = "";

   //Read data for player one
   if (saveFile.is_open())
   {
      std::getline(saveFile, dimentions);
      std::getline(saveFile, locations);
      std::getline(saveFile, theBagString);
   }
   boardDimentions = splitString(dimentions, ",");
   locationsW = splitString(locations, ", ");
   
   if (boardDimentions.size() ==2)
   {
      row = menu->charToInt(boardDimentions[0][0]);
      col = menu->charToInt(boardDimentions[1][0]);

   }
   
   for (unsigned int i =0; i < locationsW.size(); i++)
   {
      Colour colour = locationsW[i][0];
      
      Shape shape= menu->charToInt(locationsW[i][1]);
      Tile* tile = new Tile(colour, shape);
      
      row = (int)locationsW[i][3] - ASCII_A;
      //changes input location to board location
      if (locationsW[i].size() == 6)
      {
         int tens = menu->charToInt(locationsW[i][4]);
         int ones = menu->charToInt(locationsW[i][5]) - INDEXING;
         col = 10 * tens + ones;
      }
      else
      {

         col = menu->charToInt(locationsW[i][4]) - INDEXING;
      }
      Location location(row,col);
      theBoard->placeTile(tile, location);
      // delete tile;
   }
   bagTiles = splitString(theBagString, ",");

   for (unsigned int i =0; i < bagTiles.size(); i++)
   {
      Colour colour = bagTiles[i][0];
      Shape shape = menu->charToInt(bagTiles[i][1]);
      Tile* newTile = new Tile(colour, shape);
      theBag->addBack(newTile);

   }
   theBoard->setBag(theBag);


   return theBoard;
}

// Splits a string by a character inputted, returns a vector
std::vector<std::string> splitString(std::string string, std::string delim)
{
   std::vector<std::string> playerHandVector;
   if(string != "")
   {
      int start = 0;
      int end = string.find(delim);
      int length = string.size();
      std::string word;
      if(end != -1)
      {
         while(end != -1)
         {
            word = string.substr(start, end - start);
            playerHandVector.push_back(word);
            start = end + delim.size();
            end= string.find(delim, start);
         }
         
         word = string.substr(start, length);
         playerHandVector.push_back(word);
      }
      else
      {
         playerHandVector.push_back(string);
      }
   }
   
   
   return playerHandVector;
}
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
std::vector<Player* > getPlayerNames(int numPlayers, GamePlay* gameTime);
std::vector<Tile *> initialiseTileBag();
void handingTilesToPlayers(std::vector<Player* > thePlayers, Board *theBoard);
void playingTheGame(GamePlay* gameTime, int playersTurn);
Player* loadInPlayer(std::ifstream& saveFile, Menu* menu);
Board* loadInBoard(std::ifstream& saveFile, Menu* menu);
std::vector<std::string> splitString(std::string string, std::string delim);
void onePlayerTurn(GamePlay* gameTime, int currentTurn);

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
            // set the one word to the menu choice
            userInput = userString[0];
         }

         if (userInput == "1")
         {
            std::cout << "Starting a New Game" << std::endl;
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
            gameTime->getMenu()->setQuit(true);
         }
         else
         {
            std::cout << "Invalid Input!" << std::endl;
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
   std::vector<Player* > thePlayers;

   bool playersChosen = false;
   while(!playersChosen)
   {

      std::cout << std::endl;
      std::cout << "How many players want to play?" << std::endl;
      std::vector<std::string> NumPlayersVect = gameTime->getMenu()->takeLineInput(' ');
      std::string numPlayers = NumPlayersVect[0];

      if(NumPlayersVect.size() == 1 && NumPlayersVect[0].size() == 1 && (numPlayers == "2" || numPlayers == "3" || numPlayers == "4"))
      {
         if (gameTime->getMenu()->getQuit() != true)
         {
            std::string numPlayers = NumPlayersVect[0];

            int numOfPlayers = gameTime->getMenu()->charToInt(numPlayers[0]);
            std::cout << numOfPlayers << std::endl;
            thePlayers = getPlayerNames(numOfPlayers, gameTime);
            Board *board = new Board();
            std::vector<Tile *> tPtrs = initialiseTileBag();
            LinkedList* bag = new LinkedList();
            board->setBag(bag);

            for (Tile *tile : tPtrs)
            {
               board->getBag()->addFront(tile);
            }
            
            handingTilesToPlayers(thePlayers, board);
            gameTime->setPlayer(thePlayers);
            gameTime->setBoard(board);
            playingTheGame(gameTime, 0);
         }

      }
      else
      {
         std::cout << "Incorrect input" << std::endl;
      }
   }
}

std::vector<Player* > getPlayerNames(int numPlayers, GamePlay* gameTime)
{
   std::vector<Player* > thePlayers;
   for (int i = 0; i != numPlayers && gameTime->getMenu()->getQuit() == false; i++)
   {
      std::cout << "Enter a name for Player " << i + 1 << " (uppercase characters only)" << std::endl;

      std::string name = gameTime->getMenu()->getName();
      if (!gameTime->getMenu()->getQuit())
      {
         Player* player = new Player(name);
         thePlayers.push_back(player);
      }
   }
   return thePlayers;
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
void handingTilesToPlayers(std::vector<Player* > thePlayers, Board *theBoard)
{
   if (theBoard->getBag()->getSize() >= thePlayers.size() * 6)
   {
      Tile *theTile;
      for (int i = 0; i < 6; i++)
      {
         for (int j = 0; j < thePlayers.size(); j++)
         {
            theTile = theBoard->getBag()->getFront();
            thePlayers.at(j)->getHand()->addFront(theTile);
            theBoard->getBag()->removeFront();
         }
      }
   }
}

// Playing the game
void playingTheGame(GamePlay *gameTime, int playersTurn)
{
   bool gameover = false;
   int currentTurn;
   while (!gameTime->getMenu()->getQuit() && !gameover)
   {
      for(currentTurn = playersTurn; 
      currentTurn < gameTime->getPlayers().size() && !gameover && 
      !gameTime->getMenu()->getQuit(); currentTurn++)
      {
         playersTurn = 0;
         if(gameTime->getBoard()->getBag()->getSize() != 0 || 
         gameTime->legalMove(currentTurn))
         {
            onePlayerTurn(gameTime, currentTurn);
            if (gameTime->getPlayers().at(currentTurn)->getHand()->getSize() == 0)
            {
               gameover = true;
            }
         }
         else
         {
            gameover = true;

            std::cout << std::endl;
            std::cout << "You have no legal moves" << std::endl;
            std::cout << "Therefore the game is over!" << std::endl << std::endl;
         }
      }
      currentTurn = 0;
   }
   
   if (!gameTime->getMenu()->getQuit())
   {
      gameTime->handOutBonusPoints();
      
      std::sort(gameTime->getPlayers().begin(), gameTime->getPlayers().end());
      
      std::cout << "Congratulations " << gameTime->getPlayers().at(0)->getName()
       << " won with a score of: " << gameTime->getPlayers().at(0)->getScore() << std::endl;
      for(int i = 1; i < gameTime->getPlayers().size(); i++)
      {
         std::cout << "Better luck next time " << gameTime->getPlayers().at(i)->getName() 
         << " you finished with a score of: " 
         << gameTime->getPlayers().at(i)->getScore() << std::endl;
      }
      gameTime->getMenu()->setQuit(true);
   }
}

// Start of a players turn
void onePlayerTurn(GamePlay* gameTime, int playersTurn)
{
   
   std::cout << gameTime->getPlayers().at(playersTurn)->getName() << 
   " it is your turn" << std::endl;

   std::cout << gameTime->getPlayers().at(playersTurn)->getName() << "'s score: " << 
   gameTime->getPlayers().at(playersTurn)->getScore() << std::endl;

   for (int i = 0; i <gameTime->getPlayers().size(); i++)
   {
      if (i != playersTurn)
      {
         std::cout << gameTime->getPlayers().at(i)->getName() << "'s score: " << 
         gameTime->getPlayers().at(i)->getScore() << std::endl;
      }
   }
  
   gameTime->getBoard()->toString();

   gameTime->playerMove(playersTurn);
}

//Loading saved game details from a save file
void LoadGame(GamePlay* play)
{
   std::vector<Player *> thePlayers;
   std::vector<std::string> filename;
   std::string file;
   Board* theBoard;
   Player* player;
   std::string numPlayers;
   int intOfPlayers;

    std::cout << "Enter the filename from which to load a game" << std::endl;
    filename = play->getMenu()->takeLineInput(' ');
    if(filename.size() == 1 && !play->getMenu()->getQuit())
    {
      file = filename[0];
      file += ".save";
      std::ifstream saveFile(file);

      if(saveFile.is_open())
      {
         std::getline(saveFile, numPlayers);
         intOfPlayers = play->getMenu()->charToInt(numPlayers[0]);
         for (int i = 0; i < intOfPlayers; i++)
         {
            player = loadInPlayer(saveFile, play->getMenu());
            thePlayers.push_back(player);
         }

         theBoard = loadInBoard(saveFile, play->getMenu());

         std::string playerTurn = "";

         std::getline(saveFile, playerTurn);

         play->setPlayer(thePlayers);
         play->setBoard(theBoard);

         for(int i = 0; i < thePlayers.size(); i++)
         {
            if(thePlayers.at(i)->getName() == playerTurn)
            {
               playingTheGame(play, i);
            }
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
   std::string playerScore = "";
   std::string playerHand = "";
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
   }

   Player* playerOne = new Player(playerName, player1score, player1Hand);
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
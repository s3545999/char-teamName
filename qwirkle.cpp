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
void LoadGame(GamePlay *gameTime);
std::vector<Player *> getPlayerNames(int numPlayers, GamePlay *gameTime);
std::vector<Tile *> initialiseTileBag();
void handingTilesToPlayers(std::vector<Player *> thePlayers, Board *theBoard);
void playingTheGame(GamePlay *gameTime, int playersTurn);
Player *loadInPlayer(std::ifstream &saveFile, Menu *menu);
Board *loadInBoard(std::ifstream &saveFile, Menu *menu);
void onePlayerTurn(GamePlay *gameTime, int currentTurn);
bool addToHighscore(GamePlay *gameTime);

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
      if (!theMenu->getQuit())
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
            gameTime->getMenu()->printHighscores();
         }
         else if (userInput == "5")
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
void NewGame(GamePlay *gameTime)
{
   std::vector<Player *> thePlayers;

   std::cout << std::endl;
   std::cout << "How many players want to play?" << std::endl;
   std::string numPlayers;

   bool playersChosen = false;
   while (!playersChosen && !gameTime->getMenu()->getQuit())
   {
      std::vector<std::string> NumPlayersVect = gameTime->getMenu()->takeLineInput(' ');

      if (NumPlayersVect.size() == 1 && NumPlayersVect[0].size() == 1)
      {
         numPlayers = NumPlayersVect[0];
         if (numPlayers == "2" || numPlayers == "3" || numPlayers == "4")
         {
            if (gameTime->getMenu()->getQuit() != true)
            {
               std::string numPlayers = NumPlayersVect[0];

               int numOfPlayers = gameTime->getMenu()->charToInt(numPlayers[0]);
               thePlayers = getPlayerNames(numOfPlayers, gameTime);
               Board *board = new Board();
               std::vector<Tile *> tPtrs = initialiseTileBag();
               LinkedList *bag = new LinkedList();
               board->setBag(bag);

               for (Tile *tile : tPtrs)
               {
                  board->getBag()->addFront(tile);
               }
               playersChosen = true;
               handingTilesToPlayers(thePlayers, board);
               gameTime->setPlayer(thePlayers);
               gameTime->setBoard(board);
               playingTheGame(gameTime, 0);
            }
         }
      }
      if (!gameTime->getMenu()->getQuit() && !playersChosen)
      {
         std::cout << "Incorrect input" << std::endl;
      }
   }
}

std::vector<Player *> getPlayerNames(int numPlayers, GamePlay *gameTime)
{
   std::vector<Player *> thePlayers;
   for (int i = 0; i != numPlayers &&
                   gameTime->getMenu()->getQuit() == false;
        i++)
   {
      std::cout << "Enter a name for Player " << i + 1
                << " (uppercase characters only)" << std::endl;

      std::string name = gameTime->getMenu()->getName();
      if (!gameTime->getMenu()->getQuit())
      {
         Player *player = new Player(name);
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
   auto rng = std::default_random_engine{seed};
   std::shuffle(std::begin(orderedTiles), std::end(orderedTiles), rng);

   return orderedTiles;
}

// Places tiles from the bag into players hand
void handingTilesToPlayers(std::vector<Player *> thePlayers, Board *theBoard)
{
   if ((unsigned int)theBoard->getBag()->getSize() >= thePlayers.size() * 6)
   {
      Tile *theTile;
      for (int i = 0; i < 6; i++)
      {
         for (unsigned int j = 0; j < thePlayers.size(); j++)
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
      for (currentTurn = playersTurn;
           (unsigned int)currentTurn < gameTime->getPlayers().size() && !gameover &&
           !gameTime->getMenu()->getQuit();
           currentTurn++)
      {
         playersTurn = 0;
         if (gameTime->getBoard()->getBag()->getSize() != 0 ||
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
            std::cout << "Therefore the game is over!" << std::endl;
            std::cout << std::endl;
         }
      }
      currentTurn = 0;
   }

   if (!gameTime->getMenu()->getQuit())
   {
      gameTime->handOutBonusPoints();
      std::vector<Player> players;
      for (unsigned int i = 0; i < gameTime->getPlayers().size(); i++)
      {
         Player player(gameTime->getPlayers().at(i)->getName(), gameTime->getPlayers().at(i)->getScore());
         players.push_back(player);
      }
      gameTime->getBoard()->toString();
      std::cout << std::endl;

      std::sort(players.begin(), players.end());
      int draw = 0;
      for (unsigned int i = 1; i < players.size(); i++)
      {
         if (players.at(i).getScore() == players.at(0).getScore())
         {
            draw = i;
         }
      }
      if (draw > 0)
      {
         std::cout << "Congratulations ";
         for (unsigned int i = 0; i <= draw; i++)
         {

            std::cout << players.at(i).getName();
            if (i != draw)
            {
               std::cout << ", ";
            }
         }
         std::cout << " you all drew for first with a score of: "
                   << players.at(0).getScore() << std::endl;

         for (unsigned int i = draw + 1; i < players.size(); i++)
         {
            std::cout << "Unfortunate " << players.at(i).getName() << " you lost with a score of: " << players.at(i).getScore() << std::endl;
         }
      }
      else
      {
         std::cout << "Congratulations " << players.at(0).getName() << " won with a score of: "
                   << players.at(0).getScore() << std::endl;
         for (unsigned int i = 1; i < players.size(); i++)
         {
            std::cout << "Better luck next time " << players.at(i).getName()
                      << " you finished with a score of: "
                      << players.at(i).getScore() << std::endl;
         }
      }
      if (addToHighscore(gameTime))
      {
         std::cout << "Highscores Added" << std::endl;
         gameTime->getMenu()->addToHighscores(gameTime->getPlayers());
      }

      gameTime->getMenu()->setQuit(true);
   }
}

bool addToHighscore(GamePlay *gameTime)
{
   bool done = false;
   bool add = false;

   std::cout << "Do you want to add to the highscores? " << std::endl;
   std::cout << "'Yes' or 'No' " << std::endl;

   while (!done && !gameTime->getMenu()->getQuit())
   {
      std::vector<std::string> input = gameTime->getMenu()->takeLineInput(' ');
      if (input.size() == 1 && input.at(0) == "Yes")
      {
         done = true;
         add = true;
      }
      else if (input.size() == 1 && input.at(0) == "No")
      {
         done = true;
      }
      else
      {
         std::cout << "Incorrect input" << std::endl;
         std::cout << "Please input 'Yes' or 'No'" << std::endl;
      }
   }
   return add;
}

// Start of a players turn
void onePlayerTurn(GamePlay *gameTime, int playersTurn)
{

   std::cout << gameTime->getPlayers().at(playersTurn)->getName() << " it is your turn" << std::endl;

   std::cout << gameTime->getPlayers().at(playersTurn)->getName()
             << "'s score: " << gameTime->getPlayers().at(playersTurn)->getScore()
             << std::endl;

   for (unsigned int i = 0; i < gameTime->getPlayers().size(); i++)
   {
      if (i != (unsigned int)playersTurn)
      {
         std::cout << gameTime->getPlayers().at(i)->getName()
                   << "'s score: " << gameTime->getPlayers().at(i)->getScore() << std::endl;
      }
   }
   gameTime->getBoard()->toString();
   gameTime->playerMove(playersTurn);
}

//Loading saved game details from a save file
void LoadGame(GamePlay *play)
{
   std::vector<Player *> thePlayers;
   std::vector<std::string> filename;
   std::string file;
   Board *theBoard;
   Player *player;
   std::string numPlayers;
   int intOfPlayers;

   std::cout << "Enter the filename from which to load a game" << std::endl;
   filename = play->getMenu()->takeLineInput(' ');
   if (filename.size() == 1 && !play->getMenu()->getQuit())
   {
      file = filename[0];
      file += ".save";
      std::ifstream saveFile(file);

      if (saveFile.is_open())
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

         for (unsigned int i = 0; i < thePlayers.size(); i++)
         {
            if (thePlayers.at(i)->getName() == playerTurn)
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
Player *loadInPlayer(std::ifstream &saveFile, Menu *menu)
{
   //Read data for player one and two
   std::string playerName = "";
   std::string playerScore = "";
   std::string playerHand = "";
   std::vector<std::string> playerHandVector;
   int player1score = 0;

   //Read data for player one
   if (saveFile.is_open())
   {
      std::getline(saveFile, playerName);
      std::getline(saveFile, playerScore);
      std::getline(saveFile, playerHand);
   }
   player1score = menu->StringToInt(playerScore);

   playerHandVector = menu->splitString(playerHand, ",");

   LinkedList *player1Hand = new LinkedList();
   for (unsigned int i = 0; i < playerHandVector.size(); i++)
   {

      Colour colour = playerHandVector[i][0];
      Shape shape = menu->charToInt(playerHandVector[i][1]);
      Tile *newTile = new Tile(colour, shape);
      player1Hand->addBack(newTile);
   }

   Player *playerOne = new Player(playerName, player1score, player1Hand);
   return playerOne;
}
//Loads in the tiles placed on the board
Board *loadInBoard(std::ifstream &saveFile, Menu *menu)
{
   std::vector<std::string> boardDimentions;
   std::vector<std::string> locationsW;
   std::vector<std::string> bagTiles;

   std::string locations = "";
   std::string theBagString = "";
   Board *theBoard = new Board();

   LinkedList *theBag = new LinkedList();
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
   boardDimentions = menu->splitString(dimentions, ",");
   locationsW = menu->splitString(locations, ", ");

   if (boardDimentions.size() == 2)
   {
      row = menu->charToInt(boardDimentions[0][0]);
      col = menu->charToInt(boardDimentions[1][0]);
   }

   for (unsigned int i = 0; i < locationsW.size(); i++)
   {
      Colour colour = locationsW[i][0];

      Shape shape = menu->charToInt(locationsW[i][1]);
      Tile *tile = new Tile(colour, shape);

      row = (int)locationsW[i][3] - ASCII_A;
      //changes input location to board location

      int tens;
      int ones;
      int numInt;

      if (locationsW[i].size() == 6)
      {
         tens = menu->charToInt(locationsW[i][4]);
         ones = menu->charToInt(locationsW[i][5]) - INDEXING;
         numInt = (10 * tens) + (ones);
      }
      else
      {
         numInt = menu->charToInt(locationsW[i][4]) - INDEXING;
      }
      col = numInt;

      Location location(row, col);
      theBoard->placeTile(tile, location);
   }
   bagTiles = menu->splitString(theBagString, ",");

   for (unsigned int i = 0; i < bagTiles.size(); i++)
   {
      Colour colour = bagTiles[i][0];
      Shape shape = menu->charToInt(bagTiles[i][1]);
      Tile *newTile = new Tile(colour, shape);
      theBag->addBack(newTile);
   }
   theBoard->setBag(theBag);

   return theBoard;
}

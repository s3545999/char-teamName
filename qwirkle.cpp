#include "gamePlay.h"
#include "Menu.h"

#define EXIT_SUCCESS 0

void menu();

void credits();
bool NewGame(Menu* menu, GamePlay* gameTime);
std::vector<Tile *> initialiseTileBag();
bool handingTilesToPlayers(Player *player1, Player *player2, Board *theBoard);
bool playingTheGame(Player *player1, Player *player2, Board *theBoard, GamePlay* gameTime, Menu* theMenu);



int main(void)
{
   bool quit = false;
   Menu* theMenu = new Menu();
   GamePlay* gameTime = new GamePlay();

   std::cout << "Welcome to Qwirkle!" << std::endl;
   std::cout << "-------------------" << std::endl;

   std::vector<std::string> userString;
   std::string userInput = "";

   while (!quit)
   {
      theMenu->printMenu();

      userString = theMenu->takeLineInput();
      if (userString.size() == 1 && userString[0] != "Quit")
      {
         if (userString.size() == 1)
         {
            // set the one word to the menu choice
            userInput = userString[0];
         }

         if (userInput == "1")
         {
            quit = NewGame(theMenu, gameTime);
         }
         else if (userInput == "2")
         {
         }
         else if (userInput == "3")
         {
            theMenu->printCredits();
         }
         else if (userInput == "4")
         {
            quit = true;
         }
         else
         {
            std::cout << "Invalid Input!" << std::endl;
         }
      }
      else
      {
         quit = true;
      }
   }

   std::cout << "GoodBye!" << std::endl;
   return EXIT_SUCCESS;
}

bool NewGame(Menu* menu, GamePlay* gameTime)
{
   bool gameQuit = false;
   std::string name1 = "";
   std::string name2 = "";

   std::cout << std::endl;
   std::cout << "Starting a New Game" << std::endl
             << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   name1 = menu->getName();
   if(name1 != std::to_string(EOF))
   {
      Player *player1 = new Player(name1);
      std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
      std::cout << ">";
      name2 = menu->getName();
      if(name2 != std::to_string(EOF))
      {
         Player *player2 = new Player(name2);

         Board *board = new Board();

         std::vector<Tile *> tPtrs = initialiseTileBag();
         for (Tile *tile : tPtrs)
         {
            board->getBag()->addFront(tile);
         }

         handingTilesToPlayers(player1, player2, board);

         gameQuit = playingTheGame(player1, player2, board, gameTime, menu);
      }
      else
      {
         gameQuit = true;
      }
   }
   else
   {
      gameQuit = true;
   }

   return gameQuit;
}

std::vector<Tile *> initialiseTileBag()
{

   Colour tileColours[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
   Shape tileShapes[] = {CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

   std::vector<Tile *> orderedTiles;

   for (int i = 0; i < 2; i++)
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

   std::shuffle(std::begin(orderedTiles), std::end(orderedTiles), std::default_random_engine());

   return orderedTiles;
   // shuffleTiles(orderedTiles);
}

bool handingTilesToPlayers(Player *player1, Player *player2, Board *theBoard)
{
   bool success = false;
   Tile *theTile;
   if (theBoard->getBag()->size() >= 12)
   {
      for (int i = 0; i <= 6; i++)
      {
         theTile = theBoard->getBag()->getFront();
         player1->getHand()->addFront(theTile);
         theBoard->getBag()->removeFront();
         theTile = theBoard->getBag()->getFront();
         player2->getHand()->addFront(theTile);
         theBoard->getBag()->removeFront();
      }
      success = true;
   }
   return success;
}

bool playingTheGame(Player *player1, Player *player2, Board *theBoard, GamePlay* gameTime, Menu* theMenu)
{
   bool quit = false;
   int i = 0;
   while (player1->getHand()->size() != 0 && player2->getHand()->size() != 0 && !quit)
   {  
      theBoard->toString();
      std::cout << player1->getName() << "'s score: " << player1->getScore() << std::endl;
      std::cout << player2->getName() << "'s score: " << player2->getScore() << std::endl;
      quit = gameTime->playerMove(theBoard, player1, player2, theMenu);
      if (quit != true)
      {
         theBoard->toString();

         std::cout << std::endl;
         std::cout << player1->getName() << "'s score: " << player1->getScore() << std::endl;
         std::cout << player2->getName() << "'s score: " << player2->getScore() << std::endl;
         quit = gameTime->playerMove(theBoard, player2, player1, theMenu);
      }
      ++i;

   }
   return quit;
}
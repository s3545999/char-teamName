#include "GamePlay.h"
#include "Menu.h"

#define EXIT_SUCCESS 0

void menu();

void credits();
void NewGame(Menu* menu, GamePlay* gameTime);
std::vector<Tile *> initialiseTileBag();
bool handingTilesToPlayers(Player *player1, Player *player2, Board *theBoard);
void playingTheGame(Player *player1, Player *player2, Board *theBoard, GamePlay* gameTime, Menu* theMenu);



int main(void)
{
   Menu* theMenu = new Menu();
   GamePlay* gameTime = new GamePlay();

   std::cout << "Welcome to Qwirkle!" << std::endl;
   std::cout << "-------------------" << std::endl;

   std::vector<std::string> userString;
   std::string userInput = "";

   while (true)
   {
      theMenu->printMenu();

      userString = theMenu->takeLineInput();

      // Check only one word was inputted,
      if (userString.size() == 1)
      {
         // set the one word to the menu choice
         userInput = userString[0];
      }

      if (userInput == "1")
      {
         NewGame(theMenu, gameTime);
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
         theMenu->quit();
      }
      else
      {
         std::cout << "Invalid Input!" << std::endl;
      }
   }
   return EXIT_SUCCESS;
}

// void menu()
// {
//    std::cout << std::endl;
//    std::cout << "Menu" << std::endl;
//    std::cout << "----" << std::endl;
//    std::cout << "1. New Game" << std::endl;
//    std::cout << "2. Load Game" << std::endl;
//    std::cout << "3. Credits (Show student information)" << std::endl;
//    std::cout << "4. Quit" << std::endl;
//    std::cout << std::endl;
// }

// void credits()
// {
//    std::cout << "CREDITS" << std::endl;
//    std::cout << "-------" << std::endl;
// }

void NewGame(Menu* menu, GamePlay* gameTime)
{
   std::string name1 = "";
   std::string name2 = "";

   std::cout << std::endl;
   std::cout << "Starting a New Game" << std::endl
             << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   name1 = menu->getName();
   Player *player1 = new Player(name1);

   std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
   std::cout << ">";
   name2 = menu->getName();
   Player *player2 = new Player(name2);

   Board *board = new Board();

   std::vector<Tile *> tPtrs = initialiseTileBag();
   for (Tile *tile : tPtrs)
   {
      board->getBag()->addFront(tile);
   }

   handingTilesToPlayers(player1, player2, board);

   playingTheGame(player1, player2, board, gameTime, menu);

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

void playingTheGame(Player *player1, Player *player2, Board *theBoard, GamePlay* gameTime, Menu* theMenu)
{
   int i = 0;
   while (i != 6)
   {
      theBoard->toString();
      std::cout << player1->getName() << "'s score: " << player1->getScore() << std::endl;
      std::cout << player2->getName() << "'s score: " << player2->getScore() << std::endl;
      gameTime->playerMove(theBoard, player1, player2, theMenu);

      theBoard->toString();

      std::cout << std::endl;
      std::cout << player1->getName() << "'s score: " << player1->getScore() << std::endl;
      std::cout << player2->getName() << "'s score: " << player2->getScore() << std::endl;
      gameTime->playerMove(theBoard, player2, player1, theMenu);

      ++i;
   }
}
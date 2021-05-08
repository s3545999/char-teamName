#include "Board.h"


Board::Board()
{
   theBoard.resize(NO_OF_ROWS, std::vector<Tile *>(NO_OF_COLS, nullptr));
   bag = new LinkedList();
   empty = true;
}

Board::~Board()
{
}

LinkedList *Board::getBag()
{
   return bag;
}

bool Board::isSpotTaken(int Row, int Column)
{
   // std::cout << "row: "<<row<<" col: " << col <<std::endl;
   bool check = false;
   if (theBoard[Column][Row] != nullptr)
   {
      check = true;
   }

   return check;
}

void Board::toString()
{
   std::string boardPrint = " ";

   // Should print 1 through 26
   for (int j = 0; j < NO_OF_COLS; j++)
   {
      if (j < 10)
      {
         boardPrint.append("  ");
      }
      else
      {
         boardPrint.append(" ");
      }

      boardPrint.append(std::to_string(j + 1));
   }
   boardPrint += "\n";

   for (int i = 0; i < NO_OF_COLS * 3; i++)
   {
      boardPrint.append("-");
   }
   boardPrint.append("\n");
   for (int i = 0; i != NO_OF_ROWS; i++)
   {
      char letter = i + 65;
      boardPrint += letter;
      boardPrint.append(" ");
      for (int j = 0; j < NO_OF_COLS; j++)
      {
         boardPrint.append("|");

         if (theBoard[j][i] != nullptr)
         {
            std::string colour(1, theBoard[j][i]->getColour());
            boardPrint.append(colour);

            int theShape = theBoard[j][i]->getShape();
            std::string shape = std::to_string(theShape);
            boardPrint.append(shape);
         }
         else
         {
            boardPrint.append("  ");
         }
      }
      boardPrint += "|\n";
   }
   std::cout << boardPrint;
}

void Board::placeTile(Tile *newTile, int Row, int Column)
{
   theBoard[Column][Row] = newTile;
}

// void Board::setEmpty()
// {
//    empty = false;
// }

// bool Board::getEmpty()
// {

//    return empty;
// }

bool Board::checkEmpty()
{
   bool isEmpty = true;
   for(int row = 0; row < NO_OF_ROWS; row++)
   {
      for(int col = 0; col < NO_OF_COLS; col++)
      {
         if(theBoard[col][row] != nullptr)
         {
            isEmpty = false;
         }
      }
   }

   return isEmpty;
}

bool Board::emptyLocation(Location *location)
{
   bool isEmpty = false;

   if(theBoard[location->col][location->row] == nullptr)
   {
      isEmpty = true;
   }
   
   return isEmpty;
}

char Board::checkColour(Location *location)
{
   //TODO
   //WRITE CONTRACT
   return theBoard[location->col][location->row]->getColour();
}

char Board::checkShape(Location *location)
{
   //TODO
   //WRITE CONTRACT
   return theBoard[location->col][location->row]->getShape();
}

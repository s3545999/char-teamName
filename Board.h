#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H
#include <iostream>
#include <vector>

#include "Tile.h"
#include "LinkedList.h"
#include "TileCodes.h"

#define NO_OF_ROWS 26
#define NO_OF_COLS 26

class Board
{
public:
   Board();
   Board(int row, int col);
   ~Board();

   void clear();
   void setBag(LinkedList *linkedList);
   void toString();
   void placeTile(Tile *theTile, Location location);
   void setRow(int row);
   void setCol(int row);

   LinkedList *getBag();

   bool isSpotTaken(Location location);
   bool checkEmpty();
   bool emptyLocation(Location location);
   bool lineCheck(Location location, int direction, Tile *tile);

   Colour checkColour(Location location);
   Shape checkShape(Location location);

   std::string saveBoard();

   int getRows();
   int getCols();

   Tile *getTile(Location location);

   bool isOnBoard(Location location);

private:
   std::vector<std::vector<Tile *> > theBoard;
   LinkedList *bag;
   bool empty;
   int row;
   int col;
};

#endif // BOARD_H
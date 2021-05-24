
#ifndef ASSIGN1_TILECODES_H
#define ASSIGN1_TILECODES_H

#include "Tile.h"

// Colours
#define RED 'R'
#define ORANGE 'O'
#define YELLOW 'Y'
#define GREEN 'G'
#define BLUE 'B'
#define PURPLE 'P'

// Shapes
#define CIRCLE 1
#define STAR_4 2
#define DIAMOND 3
#define SQUARE 4
#define STAR_6 5
#define CLOVER 6

#define NO_OF_TILESETS  2
#define ASCII_A         65
#define ASCII_ZERO      48
#define INDEXING        1

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;

// Location struct to hold locations board positions
struct Location
{
   int row;
   int col;
   Location()
   {
   }
   Location(int row, int col)
   {
      this->row = row;
      this->col = col;
   }

   int getNextRow(int direction)
   {
      int rtnRow = row;
      if (direction == UP)
      {
         rtnRow = row - 1;
      }
      else if (direction == DOWN)
      {
         rtnRow = row + 1;
      }

      return rtnRow;
   }

   int getNextCol(int direction)
   {
      int rtnCol = col;
      if (direction == LEFT)
      {
         rtnCol = col - 1;
      }
      else if (direction == RIGHT)
      {
         rtnCol = col + 1;
      }

      return rtnCol;
   }
};

struct Move
{
   Location location;
   Tile* tile;

   Move()
   {

   }

   Move(Location location, Tile* tile)
   {
      this->location = location;
      this->tile = tile;
   }
};

#endif // ASSIGN1_TILECODES_H


#ifndef ASSIGN1_TILECODES_H
#define ASSIGN1_TILECODES_H

// Colours
#define RED    'R'
#define ORANGE 'O'
#define YELLOW 'Y'
#define GREEN  'G'
#define BLUE   'B'
#define PURPLE 'P'

// Shapes
#define CIRCLE    1
#define STAR_4    2
#define DIAMOND   3
#define SQUARE    4
#define STAR_6    5
#define CLOVER    6


const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;




struct Location
{
   int row;
   int col;
   Location()
   {}
   Location(int row, int col)
   {
      this->row = row;
      this->col = col;
   }
};



#endif // ASSIGN1_TILECODES_H

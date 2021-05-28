#ifndef GAMEPLAY
#define GAMEPLAY

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <random>

#include "Board.h"
#include "Player.h"
#include "Menu.h"


class GamePlay
{
    public:

    GamePlay();
    ~GamePlay();

   void setMenu(Menu* menu);
   Menu* getMenu();
    void setPlayer(std::vector<Player* > thePlayers);
    void setBoard(Board* theBoard);
    Board* getBoard();
    std::vector<Player* > getPlayers();
    bool check(char *s);
    void NewGame();
    void setSingleMove();
    bool getSingleMove();

    std::vector<Move> place(std::vector<std::string> wordIn, int playerTurn, std::vector<Move> theMoves);
    bool checkNextTo(std::vector<Move> theMoves, Tile* tile, Location location);

    void playerMove(int playerTurn);
    bool tileInputtedIsOkay(std::string tileString, int playerTurn);
    bool legalMove(int playerTurn);
    
    bool tileFit(Tile *tile, Location location);
    bool checkBothSides(int direction1, int direction2, Location location, Tile* tile);


    void refillHand(int playerTurn);

    bool compareTiles(std::vector<Tile*>* tileInLine);
    void checkDirection(int direction1, Location location, std::vector<Tile*>* tileInLine);
    bool checkIfNextToTiles(Location location);

    bool placeTile(std::vector<std::string> wordsIn, int playerTurn);
    bool replaceTile(std::vector<std::string> wordsIn, int playerTurn);
    bool saveGame(std::vector<std::string> wordsIn, int playerTurn);

    int convertToRow(char row);
    int convertToCol(char col);

    Tile *turnInputToTile(std::string tiledata);

    Location convertInputLoc(std::string inputLocation);
    bool inputtedLocIsOkay(std::string location);
    int score(std::vector<Move> theMoves);

    void HandPlayerTile(int playerTurn);

    int scoreDirection(int direction, Location location);
    void handOutBonusPoints();

    private:

    Menu* menu;
    Board* theBoard;
    std::vector<Player* > thePlayers;
    bool singleTurn;
};




#endif //GAMEPLAY
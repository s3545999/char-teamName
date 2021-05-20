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
    
    bool playerMove(int playerTurn);
    bool tileInputtedIsOkay(std::string tileString, int playerTurn);
    bool legalMove(int playerTurn);
    
    bool tileFit(Tile *tile, Location location);
    bool checkBothSides(int direction1, int direction2, Location location, Tile* tile);

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

    int score(Location location);

    void HandPlayerTile(int playerTurn);

    int scoreDirection(int direction, Location location);
    void handOutBonusPoints();

    private:

    Menu* menu;
    Board* theBoard;
    std::vector<Player* > thePlayers;
};




#endif //GAMEPLAY
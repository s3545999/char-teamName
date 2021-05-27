#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "TileCodes.h"
#include "Player.h"

class Menu
{
public:
    Menu();

    bool getQuit();
    void setQuit(bool quitBool);
    void printMenu();
    void printCredits();

    std::vector<std::string> takeLineInput(char charToSplit);

    std::string getName();
    int charToInt(char character);
    int StringToInt(std::string number);

    void printHighscores();
    std::vector<Player> readInHighscores();
    void addToHighscores(std::vector<Player *> newPlayers);

    std::vector<std::string> splitString(std::string string, std::string delim);

private:
    bool quit;
};

#endif //MENU_H
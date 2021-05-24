#include "Menu.h"


#include <iostream>
#include <fstream>

Menu::Menu()
{
   quit = false;
}


bool Menu::getQuit()
{
   return quit;
}
void Menu::setQuit(bool quitBool)
{
   quit = quitBool;
}
// Prints out a menu
void Menu::printMenu()
{
   std::cout << std::endl;
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Highscores" << std::endl;
   std::cout << "5. Quit" << std::endl;
   std::cout << std::endl;
}

// Prints the credits 
void Menu::printCredits()
{
   std::cout << "CREDITS" << std::endl;
   std::cout << "-------------------------------------------" << std::endl;
   std::cout << "Name:       " << "Aidan McGinniss" << std::endl;
   std::cout << "Student Id: " << "s3545999" << std::endl;
   std::cout << "Email:      " << "s3545999@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name:       " << "Ashwin Venkita Subharaman" << std::endl;
   std::cout << "Student Id: " << "s3783614" << std::endl;
   std::cout << "Email:      " << "s3783614@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name:       " << "Shewit Tikue" << std::endl;
   std::cout << "Student Id: " << "s3545853" << std::endl;
   std::cout << "Email:      " << "s3545853@student.rmit.edu.au" << std::endl;
   std::cout << std::endl;
   std::cout << "Name:       " << "Hamida Ehsani" << std::endl;
   std::cout << "Student Id: " << "s3653728" << std::endl;
   std::cout << "Email:      " << "s3653728@student.rmit.edu.au" << std::endl;
   std::cout << "-------------------------------------------" << std::endl;
   
}

// Converts a char of an integer to an integer
int Menu::charToInt(char character)
{
   return (int)character - ASCII_ZERO;
}


int Menu::StringToInt(std::string number)
{
   int hundreds;
   int tens;
   int ones;
   int numInt;

   if (number.size() == 3)
   {
      hundreds = charToInt(number[0]);
      tens = charToInt(number[1]);
      ones = charToInt(number[2]);
      numInt = (100*hundreds)+(10 * tens) + (ones);
   }
   else if(number.size() == 2)
   {
      tens = charToInt(number[0]);
      ones = charToInt(number[1]);
      numInt = (10 * tens) + (ones);
   }
   else
   {
      numInt = charToInt(number[0]);
   }

   return numInt;
}

// Asks the User for an Input and takes an entire line with spaces
// Splits the input to individual words by spaces
// Puts each word in order into a vector
// Returns the vector
std::vector<std::string> Menu::takeLineInput(char charToSplit)
{
   std::vector<std::string> wordsIn;
   std::string theMove = "";

   std::cout << "> ";
   std::getline(std::cin, theMove);

   // Check that eof character not inputted
   if (!std::cin.eof())
   {
      // Take the line inputted and split by spaces to individual words
      std::stringstream check1(theMove);
      std::string tmpString = "";
      while (getline(check1, tmpString, charToSplit))
      {
         wordsIn.push_back(tmpString);
      }
   }
   else
   {
      quit = true;
   }
   return wordsIn;
}

std::string Menu::getName()
{
   std::vector<std::string> wordsIn;
   std::string name;

   bool checker = false;

   while (checker == false)
   {
      checker = true;
      std::cout << "Enter your name (ONLY CAPITALS AND NO SPACES)" 
      << std::endl;

      wordsIn = takeLineInput(' ');
      if(wordsIn.size() ==1 && wordsIn[0] != std::to_string(EOF))
      {
         // Check only one word inputted
         if (wordsIn.size() == 1)
         {
            name = wordsIn[0];

            for (unsigned int i = 0; i < name.length(); i++)
            {

               if (name[i] < 65 || name[i] > 90)
               {
                  checker = false;
               }
            }
            if (checker == false)
            {
               std::cout << "Invalid Input!" << std::endl;
               std::cout << "Please enter ONLY CAPITAL LETTERS!" << std::endl;
               std::cout << std::endl;
            }
         }
         else
         {
            checker = false;
            std::cout << "Invalid Input!" << std::endl;
            std::cout << "Please enter only one name!" << std::endl;
            std::cout << std::endl;
         }
      }
      else
      {
         name = std::to_string(EOF);
         checker = true;
      }
      
   }
   return name;
}

void Menu::printHighscores()
{
   std::cout << "Highscores:" << std::endl;
   std::vector<Player> players = readInHighscores();
   std::sort(players.begin(), players.end());
   for(int i = 0; i < players.size(); i++)
   {
      std::cout << players.at(i).getName() << " " <<
      players.at(i).getScore() << std::endl;
   }
   std::cout << std::endl;
}

std::vector<Player> Menu::readInHighscores()
{
   std::string player;
   int score;
   std::vector<std::string> playerString;
   std::vector<Player> players;
   std::ifstream highscore("highscores.txt");
   while(highscore.peek() != EOF)
   {
      std::getline(highscore, player);
      playerString = splitString(player, " ");
      score = StringToInt(playerString.at(1));
      Player player(playerString.at(0), score);
      players.push_back(player);
   }
   return players;
}

void Menu::addToHighscores(std::vector<Player* > newPlayers)
{
   std::vector<Player> oldPlayers = readInHighscores();

   for(int i = 0; i < newPlayers.size(); i++)
   {
      Player player (newPlayers.at(i)->getName(), newPlayers.at(i)->getScore());
      oldPlayers.push_back(player);
   }
   std::sort(oldPlayers.begin(), oldPlayers.end());

   std::ofstream MyFile("highscores.txt");

   if(!MyFile.fail())
   {
      for(int i = 0; i < oldPlayers.size() && i < 10; i++)
      {
         MyFile << oldPlayers.at(i).getName() << ": " << oldPlayers.at(i).getScore() << std::endl;
      }
   }
}

// Splits a string by a character inputted, returns a vector
std::vector<std::string> Menu::splitString(std::string string, 
std::string delim)
{
   std::vector<std::string> stringVector;
   if(string != "")
   {
      int start = 0;
      int end = string.find(delim);
      int length = string.size();
      std::string word;
      if(end != -1)
      {
         while(end != -1)
         {
            word = string.substr(start, end - start);
            stringVector.push_back(word);
            start = end + delim.size();
            end= string.find(delim, start);
         }
         
         word = string.substr(start, length);
         stringVector.push_back(word);
      }
      else
      {
         stringVector.push_back(string);
      }
   }

   return stringVector;
}
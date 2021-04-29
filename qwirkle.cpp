#include "LinkedList.h"

#include <iostream>
#include <string>

#include "Player.h"
#include "Bag.h"
#include "Board.h"

#define EXIT_SUCCESS 0

bool check(char *s);
bool NewGame();
std::string getName();

void menu();
void Quit();
void credits();

int main(void)
{
   std::cout << "Welcome to Qwirkle!" << std::endl;
   std::cout << "-------------------" << std::endl;

   std::string userInput = "";
   bool exit = false;
   while (exit == false)
   {
      menu();
      std::cout << ">";
      // char opt[10];
      // if (!std::cin.get(opt, 10))
      // {
      //    Quit();
      //    return false;
      // }
      // getchar();
      std::cin >> userInput;
      // if (strlen(opt) != 1)
      // {
      //    std::cout << "Invalid Input" << std::endl;
      //    continue;
      // }
      // if (!(userInput[0] >= '1' && opt[0] <= '4'))
      // {
      //    std::cout << "Invalid Input" << std::endl;
      //    continue;
      // }

      // int code = opt[0] - '0';
      if (userInput == "1")
      {
         // if (!NewGame())
         NewGame();
      }
      else if (userInput == "2")
      {
      }
      else if (userInput == "3")
      {
         credits();
      }
      else if (userInput == "4")
      {
         Quit();
         exit = true;
      }
      else
      {
         std::cout << "Invalid Input!" << std::endl;
      }
   }

   return EXIT_SUCCESS;
}

void menu()
{
   std::cout << std::endl;
   std::cout << "Menu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
   std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
   std::cout << std::endl;
}

void Quit()
{
   // std::cout << std::endl;
   std::cout << "Goodbye" << std::endl;
}

// bool check(char *s)
// {
//    // int l = strlen(s);
//    for (int i = 0; i < l; i++)
//    {
//       if (!(s[i] >= 'A' && s[i] <= 'Z'))
//       {
//          std::cout << "Invalid Input" << std::endl;
//          return false;
//       }
//    }
//    return true;
// }

bool NewGame()
{
   std::string name1 = "";
   std::string name2 = "";

   std::cout << std::endl;
   std::cout << "Starting a New Game" << std::endl
             << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   std::cout << ">";
   name1 = getName();
   Player* player1 = new Player(name1);
   std::cout << player1->getName() << std::endl;
   // if (!std::cin.getline(name1, 20))
   // {
   //    Quit();
   //    return false;
   // }
   // while (!check(name1))
   // {
   //    std::cout << "Enter a name for palyer 1 (uppercase characters only)" << std::endl;
   //    std::cout << ">";
   //    if (!std::cin.getline(name1, 10))
   //    {
   //       Quit();
   //       return false;
   //    }
   // }
   std::cout << "Enter a name for player 2 (uppercase characters only)" << std::endl;
   std::cout << ">";
   name2 = getName();
   Player* player2 = new Player(name2);
   std::cout << player2->getName() << std::endl;
   // if (!std::cin.getline(name2, 10))
   // {
   //    Quit();
   //    return false;
   // }
   // while (!check(name2))
   // {
   //    std::cout << "Enter a name for palyer 2 (uppercase characters only)" << std::endl;
   //    std::cout << ">";
   //    if (!std::cin.getline(name2, 10))
   //    {
   //       Quit();
   //       return false;
   //    }
   // }
   Bag* bag = new Bag();
   bag->initialiseTileBag();
   bag->printBag();
   return true;
}

std::string getName()
{
   std::string name;

   bool checker = false;
   while (checker == false)
   {
      // std::cout << "Enter your name (CAPITALS AND NO SPACES)" << std::endl;
      std::cin >> name;
      checker = true;
      for (unsigned int i = 0; i < name.length(); i++)
      {

         if (name[i] < 65 || name[i] > 90)
         {
            checker = false;
         }
         
      }
      if(checker == false)
      {
         std::cout << "Invalid Input!" << std::endl;
         std::cout << "Please enter ONLY CAPITAL LETTERS!" << std::endl;
         std::cout << ">" ;
      }
   }
   return name;
}

void credits()
{
   std::cout << "CREDITS" << std::endl;
   std::cout << "-------" << std::endl;
}
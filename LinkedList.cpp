#include "LinkedList.h"

#include <iostream>

LinkedList::LinkedList()
{
   head = nullptr;
   size = 0;
}

LinkedList::~LinkedList()
{
   clear();
}

// Return the size of the linked list
int LinkedList::getSize()
{
   return size;
}

// Removes all nodes and tiles from the linked list
void LinkedList::clear()
{
   while (head != nullptr)
   {
      delete head->tile;
      removeFront();
   }
   size = 0;
}

// Get a tile at a node index
Tile *LinkedList::get(int index)
{
   int counter = 0;
   Node *current = head;

   Tile *returnTile;

   if (index < size)
   {
      while (counter < index)
      {
         counter++;
         current = current->next;
      }
      returnTile = current->tile;
   }
   else
   {
      throw std::out_of_range("Linked list get - Index out of range");
   }

   return returnTile;
}

// Place a tile at the front of the linkedlist
void LinkedList::addFront(Tile *tile)
{
   Node *addedTile = new Node(tile, nullptr);

   if (head == nullptr)
   {
      head = addedTile;
   }
   else
   {
      addedTile->next = head;
      head = addedTile;
   }
   size++;
}

// Add a tile at the back of the linked list
void LinkedList::addBack(Tile *tile)
{
   Node *addedTile = new Node(tile, nullptr);

   if (head == nullptr)
   {
      head = addedTile;
   }
   else
   {
      Node *temp = head;
      while (temp->next != nullptr)
      {

         temp = temp->next;
      }

      temp->next = addedTile;
   }
   size++;
}

// Remove the front node and tile
void LinkedList::removeFront()
{
   if (head != nullptr)
   {
      Node *secondaryElement = head->next;
      delete head;
      head = secondaryElement;
   }
   size--;
}

// Remove the last tile in the linkedlist
void LinkedList::removeBack()
{
   if (head != nullptr)
   {
      Node *current = head;
      Node *previous = nullptr;

      while (current->next != nullptr)
      {
         previous = current;
         current = current->next;
      }

      previous->next = nullptr;
      delete current;
   }
   size--;
}

// remove a tile at index
void LinkedList::removeAt(int index)
{
   // std::cout << "size: " <<size()<<std::endl;
   if (index >= 0 && index < size)
   {

      if (head != nullptr)
      {
         int counter = 0;
         Node *current = head;
         Node *previous = nullptr;

         while (counter != index)
         {
            ++counter;
            previous = current;
            current = current->next;
         }

         if (previous == nullptr)
         {
            head = current->next;
         }
         else
         {
            previous->next = current->next;
         }
         delete current->tile;
         delete current;
         size--;
      }
   }
}

// Returns the front tile in a linkedList
Tile *LinkedList::getFront()
{
   Tile *returnTile = nullptr;
   if (head != nullptr)
   {
      returnTile = head->tile;
   }

   return returnTile;
}

// Finds if a tile in the linkedlist and returns whether it is successful
bool LinkedList::isInLinkedList(Tile *tile)
{
   bool isIn = false;
   Node *current = head;

   while (current != nullptr)
   {
      if (current->tile->compareTile(tile))
      {
         isIn = true;
      }
      current = current->next;
   }

   return isIn;
}

// Return the index of a tile in the linkedlist
int LinkedList::findSpecificTile(Tile *tile)
{
   int index = -1;

   Node *current = head;

   int counter = 0;
   while (index == -1)
   {
      if (current->tile->compareTile(tile))
      {

         index = counter;
      }
      ++counter;
      current = current->next;
   }

   return index;
}

// Prints a string of the linkedlist
std::string LinkedList::llToString()
{

   std::string llTiles = "";
   for (int i = 0; i < size; i++)
   {

      llTiles += get(i)->getColour();
      llTiles += std::to_string(get(i)->getShape());
      if (i != size - 1)
      {
         llTiles += ",";
      }
   }
   return llTiles;
}

#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Bag.h"

class LinkedList {
public:

   LinkedList();
   LinkedList(LinkedList& other);
   ~LinkedList();

   int size();
   void clear();
   Tile* get(int index);

   void addFront(Tile* tile);
   void addBack(Tile* tile);
   void removeFront();
   void removeBack();

   Tile* getFront();

   void printLinkedList();


private:
   Node* head;
};

#endif // ASSIGN2_LINKEDLIST_H

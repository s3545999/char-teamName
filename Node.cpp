
#include "Node.h"

Node::Node()
{
   this->tile = nullptr;
   this->next = nullptr;
}

Node::Node(Tile *tile, Node *next)
{

   this->tile = tile;
   this->next = next;
}

//Returns shallow copy for now
Node::Node(Node &other) : tile(other.tile),
                          next(other.next)
{
}

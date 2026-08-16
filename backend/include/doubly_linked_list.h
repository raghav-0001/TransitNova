#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "route.h"

struct DNode
{
    Route data;

    DNode *prev;
    DNode *next;

    DNode(const Route &route);
};

class DoublyLinkedList
{

private:
    DNode *head;
    DNode *tail;

public:
    DoublyLinkedList();

    ~DoublyLinkedList();

    bool insert(const Route &route);

    Route *search(int routeID);

    bool remove(int routeID);

    void displayForward() const;

    void displayBackward() const;

    bool isEmpty() const;

    int size() const;

    void clear();
};

#endif
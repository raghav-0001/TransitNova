#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "bus.h"

struct Node
{
    Bus data;
    Node *next;

    Node(const Bus &bus);
};

class LinkedList
{
private:
    Node *head;

public:
    LinkedList();
    ~LinkedList();

    // Priority Queue Operations
    bool insert(const Bus &bus);
    Bus removeFront();

    // Other Operations
    Bus *search(int busID);
    Bus *searchByNumber(const std::string &busNumber);
    bool remove(int busID);

    void display() const;

    bool isEmpty() const;
    int size() const;

    void clear();

    Node *getHead();
    const Node *getHead() const;
};

#endif
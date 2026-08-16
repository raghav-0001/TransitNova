#include "../include/doubly_linked_list.h"

#include <iostream>

using namespace std;

DNode::DNode(const Route &route)
{
    data = route;

    prev = nullptr;
    next = nullptr;
}

DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
}

DoublyLinkedList::~DoublyLinkedList()
{
    clear();
}

bool DoublyLinkedList::insert(const Route &route)
{

    DNode *newNode = new DNode(route);

    if (head == nullptr)
    {
        head = tail = newNode;
        return true;
    }

    tail->next = newNode;

    newNode->prev = tail;

    tail = newNode;

    return true;
}

Route *DoublyLinkedList::search(int routeID)
{

    DNode *temp = head;

    while (temp != nullptr)
    {

        if (temp->data.getID() == routeID)
        {
            return &temp->data;
        }

        temp = temp->next;
    }

    return nullptr;
}

bool DoublyLinkedList::remove(int routeID)
{

    DNode *temp = head;

    while (temp != nullptr)
    {

        if (temp->data.getID() == routeID)
        {

            if (temp == head)
                head = temp->next;

            if (temp == tail)
                tail = temp->prev;

            if (temp->prev)
                temp->prev->next = temp->next;

            if (temp->next)
                temp->next->prev = temp->prev;

            delete temp;

            return true;
        }

        temp = temp->next;
    }

    return false;
}

void DoublyLinkedList::displayForward() const
{

    if (head == nullptr)
    {
        cout << "No route available\n";
        return;
    }

    cout << "\n===== ROUTE WEST TO EAST =====\n";

    DNode *temp = head;

    while (temp)
    {

        cout
            << temp->data.getID()
            << " : "
            << temp->data.getName()
            << endl;

        temp = temp->next;
    }
}

void DoublyLinkedList::displayBackward() const
{

    if (tail == nullptr)
    {
        cout << "No route available\n";
        return;
    }

    cout << "\n===== ROUTE EAST TO WEST =====\n";

    DNode *temp = tail;

    while (temp)
    {

        cout
            << temp->data.getID()
            << " : "
            << temp->data.getName()
            << endl;

        temp = temp->prev;
    }
}

bool DoublyLinkedList::isEmpty() const
{
    return head == nullptr;
}

int DoublyLinkedList::size() const
{

    int count = 0;

    DNode *temp = head;

    while (temp)
    {
        count++;

        temp = temp->next;
    }

    return count;
}

void DoublyLinkedList::clear()
{

    while (head)
    {

        DNode *temp = head;

        head = head->next;

        delete temp;
    }

    tail = nullptr;
}
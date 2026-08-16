#include "../include/linked_list.h"

#include <iostream>

using namespace std;

Node::Node(const Bus &bus)
{
    data = bus;
    next = nullptr;
}

LinkedList::LinkedList()
{
    head = nullptr;
}

LinkedList::~LinkedList()
{
    clear();
}

// Priority Insert
bool LinkedList::insert(const Bus &bus)
{
    Node *newNode = new Node(bus);

    if (head == nullptr)
    {
        head = newNode;
        return true;
    }

    Node *current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;

    return true;
}

// Remove Highest Priority Bus
Bus LinkedList::removeFront()
{
    if (head == nullptr)
    {
        cout << "Priority Queue Empty.\n";
        return Bus();
    }

    Node *temp = head;

    Bus bus = temp->data;

    head = head->next;

    delete temp;

    return bus;
}

Bus *LinkedList::search(int busID)
{
    Node *temp = head;

    while (temp != nullptr)
    {
        if (temp->data.getID() == busID)
            return &temp->data;

        temp = temp->next;
    }

    return nullptr;
}

Bus *LinkedList::searchByNumber(const std::string &busNumber)
{
    Node *temp = head;

    while (temp != nullptr)
    {
        if (temp->data.getBusNumber() == busNumber)
            return &temp->data;

        temp = temp->next;
    }

    return nullptr;
}

bool LinkedList::remove(int busID)
{
    if (head == nullptr)
        return false;

    if (head->data.getID() == busID)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Node *prev = head;
    Node *curr = head->next;

    while (curr != nullptr)
    {
        if (curr->data.getID() == busID)
        {
            prev->next = curr->next;
            delete curr;
            return true;
        }

        prev = curr;
        curr = curr->next;
    }

    return false;
}

void LinkedList::display() const
{
    if (head == nullptr)
    {
        cout << "Queue is Empty.\n";
        return;
    }

    cout << "\n===== Priority Queue =====\n";

    Node *temp = head;

    while (temp != nullptr)
    {
        cout
            << "ID: " << temp->data.getID()
            << " | Bus: " << temp->data.getBusNumber()
            << " | Stop: " << temp->data.getCurrentStop()
            << " | Direction: " << temp->data.getDirection()
            << " | Priority: " << temp->data.getPriority()
            << endl;

        temp = temp->next;
    }

    cout << endl;
}

bool LinkedList::isEmpty() const
{
    return head == nullptr;
}

int LinkedList::size() const
{
    int count = 0;

    Node *temp = head;

    while (temp != nullptr)
    {
        count++;
        temp = temp->next;
    }

    return count;
}

void LinkedList::clear()
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}

Node *LinkedList::getHead()
{
    return head;
}

const Node *LinkedList::getHead() const
{
    return head;
}
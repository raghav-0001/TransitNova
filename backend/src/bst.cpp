#include "../include/bst.h"

#include <iostream>

using namespace std;

BSTNode::BSTNode(const Bus &bus)
{
    data = bus;
    left = nullptr;
    right = nullptr;
}

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    clear(root);
}

void BST::clear(BSTNode *node)
{
    if (node == nullptr)
        return;

    clear(node->left);
    clear(node->right);

    delete node;
}

void BST::insert(const Bus &bus)
{
    root = insert(root, bus);
}

BSTNode *BST::insert(BSTNode *node, const Bus &bus)
{
    if (node == nullptr)
    {
        return new BSTNode(bus);
    }

    if (bus.getID() < node->data.getID())
    {
        node->left = insert(node->left, bus);
    }
    else if (bus.getID() > node->data.getID())
    {
        node->right = insert(node->right, bus);
    }

    return node;
}

Bus *BST::search(int busID)
{
    BSTNode *result = search(root, busID);

    if (result == nullptr)
        return nullptr;

    return &result->data;
}

BSTNode *BST::search(BSTNode *node, int busID)
{
    if (node == nullptr)
        return nullptr;

    if (busID == node->data.getID())
        return node;

    if (busID < node->data.getID())
        return search(node->left, busID);

    return search(node->right, busID);
}

BSTNode *BST::findMin(BSTNode *node)
{
    while (node != nullptr && node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

bool BST::remove(int busID)
{
    if (search(busID) == nullptr)
    {
        return false;
    }

    root = remove(root, busID);

    return true;
}

BSTNode *BST::remove(BSTNode *node, int busID)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (busID < node->data.getID())
    {
        node->left = remove(node->left, busID);
    }
    else if (busID > node->data.getID())
    {
        node->right = remove(node->right, busID);
    }
    else
    {
        // No child
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }

        // One child
        if (node->left == nullptr)
        {
            BSTNode *temp = node->right;
            delete node;
            return temp;
        }

        if (node->right == nullptr)
        {
            BSTNode *temp = node->left;
            delete node;
            return temp;
        }

        // Two children
        BSTNode *temp = findMin(node->right);

        node->data = temp->data;

        node->right = remove(node->right, temp->data.getID());
    }

    return node;
}

void BST::inorder() const
{
    inorder(root);
}

void BST::preorder() const
{
    preorder(root);
}

void BST::postorder() const
{
    postorder(root);
}

void BST::inorder(BSTNode *node) const
{
    if (node == nullptr)
        return;

    inorder(node->left);

    cout << "ID: " << node->data.getID()
         << " | Bus: " << node->data.getBusNumber()
         << " | Route: " << node->data.getRoute()
         << " | Stop: " << node->data.getCurrentStop()
         << " | Direction: " << node->data.getDirection()
         << " | Priority: " << node->data.getPriority()
         << endl;

    inorder(node->right);
}

void BST::preorder(BSTNode *node) const
{
    if (node == nullptr)
        return;

    cout << "ID: " << node->data.getID()
         << " | Bus: " << node->data.getBusNumber()
         << " | Route: " << node->data.getRoute()
         << " | Stop: " << node->data.getCurrentStop()
         << " | Direction: " << node->data.getDirection()
         << " | Priority: " << node->data.getPriority()
         << endl;

    preorder(node->left);
    preorder(node->right);
}

void BST::postorder(BSTNode *node) const
{
    if (node == nullptr)
        return;

    postorder(node->left);
    postorder(node->right);

    cout << "ID: " << node->data.getID()
         << " | Bus: " << node->data.getBusNumber()
         << " | Route: " << node->data.getRoute()
         << " | Stop: " << node->data.getCurrentStop()
         << " | Direction: " << node->data.getDirection()
         << " | Priority: " << node->data.getPriority()
         << endl;
}
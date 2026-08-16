#ifndef BST_H
#define BST_H

#include "bus.h"

struct BSTNode
{
    Bus data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(const Bus &bus);
};

class BST
{
private:
    BSTNode *root;

    BSTNode *insert(BSTNode *node, const Bus &bus);
    BSTNode *search(BSTNode *node, int busID);
    BSTNode *remove(BSTNode *node, int busID);
    BSTNode *findMin(BSTNode *node);

    void inorder(BSTNode *node) const;
    void preorder(BSTNode *node) const;
    void postorder(BSTNode *node) const;

    void clear(BSTNode *node);

public:
    BST();
    ~BST();

    void insert(const Bus &bus);

    Bus *search(int busID);

    bool remove(int busID);

    void inorder() const;
    void preorder() const;
    void postorder() const;
};

#endif
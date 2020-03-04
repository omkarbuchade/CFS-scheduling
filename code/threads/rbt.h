/** C++ implementation for Red-Black Tree Insertion
This code is adopted from the code provided by Dinesh Khandelwal, available at
https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/ **/

// Name: Omkar Buchade: Added methods required to get data from RB tree for CFS scheduling

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H
#include "thread.h"
#include "list.h"

enum Color {RED, BLACK, DOUBLE_BLACK};

struct Node
{
    int vRunTime;
    int wait;
    int color;
    Node *left, *right, *parent;
    Thread *th;

    explicit Node(Thread *);
};

class RBTree
{
    private:
        Node *root;
        int size;
    protected:
        void rotateLeft(Node *&);
        void rotateRight(Node *&);
        void fixInsertRBTree(Node *&);
        void fixDeleteRBTree(Node *&);
        List <Thread *> * inorderBST(Node *&);
        void preorderBST(Node *&);
        int getColor(Node *&);
        void setColor(Node *&, int);
        Node *minValueNode(Node *&);
        Node *maxValueNode(Node *&);
        Node* insertBST(Node *&, Node *&);
        Node* deleteBST(Node *&, int);
        int getBlackHeight(Node *);
        void updateWaitVar(Node *&);
    public:
        RBTree();
        void insertValue(Thread *); //insert a value into RB tree
        void deleteMinValue(); //deelete smallest node from RB tree
       // void merge(RBTree);
        List <Thread *> * inorder();
        int getTreeSize(); //get number of elements in the RB tree
        void setTreeSize(bool); //increment/decrement RBtree size

        void preorder();
        Node* getMinNode(); //get smallest node from TB tree (Leftmost leaf node)
        void updateWait(); //update the wait time for each node in rbt to calculate the decay
};

#endif //RED_BLACK_TREE_RBTREE_H
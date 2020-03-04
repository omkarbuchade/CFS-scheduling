
#include "rbt.h"
#include <iostream>
using namespace std;


//method to divide number 
float divide (int x,int y) 
{  
    int c1=0;  
    int c2=0;  
    if(x>y)     
    {     
        while (x-y > 0)     
        { 
            x=x-y;         
            c1++;     
        }     
        x=x*100;     
        while(x-y >= 0)     
        {       
            c2++;
            x=x-y;     
        }     
        return (c1+ 0.01*c2);  
        }  
        else if (x==y)     
            return 1; 
        else 
        {     
            x=x*100;     
            while(x-y >= 0)     
            {                  
                c2++;         
                x=x-y;     
            }     
            return (0.01*c2); 
        } 
    }

Node::Node(Thread *thrd) {
    color = RED;
    left = right = parent = NULL;
    this->th=thrd;
    this->vRunTime = thrd->getExecution_time()* divide(1,thrd->getWait());

    if(th->getName()=="postal worker" || th->getName()=="postal worker " || th->getName()=="main" )
    {
    this->vRunTime = 10000;
    }

    wait=1;
    thrd->setWait(1);
} 

RBTree::RBTree() {
    root = NULL;
    size=0;
}


int RBTree::getColor(Node *&node) {
    if (node == NULL)
        return BLACK;

    return node->color;
}

void RBTree::setColor(Node *&node, int color) {
    if (node == NULL)
        return;

    node->color = color;
}

Node* RBTree::insertBST(Node *&root, Node *&ptr) {
    if (root == NULL)
        return ptr;

    if (ptr->vRunTime <= root->vRunTime) {
        root->left = insertBST(root->left, ptr);
        root->left->parent = root;
    } else if (ptr->vRunTime > root->vRunTime) {
        root->right = insertBST(root->right, ptr);
        root->right->parent = root;
    }

    return root;
}

void RBTree::insertValue(Thread *thrd) {
    Node *node = new Node(thrd);
    root = insertBST(root, node);
    fixInsertRBTree(node);
}

void RBTree::rotateLeft(Node *&ptr) {
    Node *right_child = ptr->right;
    ptr->right = right_child->left;

    if (ptr->right != NULL)
        ptr->right->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == NULL)
        root = right_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;
    ptr->parent = right_child;
}

void RBTree::rotateRight(Node *&ptr) {
    Node *left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != NULL)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == NULL)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(Node *&ptr) {
    Node *parent = NULL;
    Node *grandparent = NULL;
    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
        parent = ptr->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node *uncle = grandparent->right;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->right) {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        } else {
            Node *uncle = grandparent->left;
            if (getColor(uncle) == RED) {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            } else {
                if (ptr == parent->left) {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }
    setColor(root, BLACK);
}

void RBTree::fixDeleteRBTree(Node *&node) {
    if (node == NULL)
        return;

    if (node == root) {
        root = NULL;
        return;
    }

    if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
        Node *child = node->left != NULL ? node->left : node->right;

        if (node == node->parent->left) {
            node->parent->left = child;
            if (child != NULL)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        } else {
            node->parent->right = child;
            if (child != NULL)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
    } else {
        Node *sibling = NULL;
        Node *parent = NULL;
        Node *ptr = node;
        setColor(ptr, DOUBLE_BLACK);
        while (ptr != root && getColor(ptr) == DOUBLE_BLACK) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                } else {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                        setColor(sibling, RED);
                        if(getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->right) == BLACK) {
                            setColor(sibling->left, BLACK);
                            setColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            } else {
                sibling = parent->left;
                if (getColor(sibling) == RED) {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                } else {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getColor(sibling->left) == BLACK) {
                            setColor(sibling->right, BLACK);
                            setColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
        delete(node);
        setColor(root, BLACK);
    }
}

Node* RBTree::deleteBST(Node *&root, int vRunTime) {
    if (root == NULL)
        return root;

    if (vRunTime < root->vRunTime)
        return deleteBST(root->left, vRunTime);

    if (vRunTime > root->vRunTime)
        return deleteBST(root->right, vRunTime);

    if (root->left == NULL || root->right == NULL)
        return root;

    Node *temp = minValueNode(root->right);
    root->vRunTime = temp->vRunTime;
    return deleteBST(root->right, temp->vRunTime);
}

void RBTree::deleteMinValue() {
    Node *node = minValueNode(root);
    fixDeleteRBTree(node);
}

List <Thread *> * RBTree::inorderBST(Node *&ptr) {
    
    List <Thread *> *threadList = new List <Thread *> ();
    
    if (ptr == NULL)
    {
        return NULL;
    }
    inorderBST(ptr->left);
    std::cout <<"thread name: "<<ptr->th->getName()<<" | thread id: "<<ptr->th->getTid() <<" | vRunTime: "<< ptr->vRunTime << " | wait/decay factor: " << ptr->wait << "\n";
    threadList->Append(ptr->th);
    inorderBST(ptr->right);
    return threadList;
}

List <Thread *> * RBTree::inorder() {
    std::cout<<"-----------------------------------------------------------------------------------------------\n";
    return (inorderBST(root));

}

int RBTree::getTreeSize()
{
    return size;
}

void RBTree::setTreeSize(bool inc)
{
    if(inc)
        size++;
    else
        size--;
}



void RBTree::preorderBST(Node *&ptr) {
    if (ptr == NULL)
        return;

    cout << ptr->vRunTime << " " << ptr->color << endl;
    preorderBST(ptr->left);
    preorderBST(ptr->right);
}

void RBTree::preorder() {
    preorderBST(root);
    cout << "-------" << endl;
}

Node *RBTree::minValueNode(Node *&node) {

    Node *ptr = node;

    while (ptr->left != NULL)
        ptr = ptr->left;

    return ptr;
}

Node* RBTree::maxValueNode(Node *&node) {
    Node *ptr = node;

    while (ptr->right != NULL)
        ptr = ptr->right;

    return ptr;
}

int RBTree::getBlackHeight(Node *node) {
    int blackheight = 0;
    while (node != NULL) {
        if (getColor(node) == BLACK)
            blackheight++;
        node = node->left;
    }
    return blackheight;
}

Node* RBTree::getMinNode()
{
    if(root ==NULL)
        return NULL;
    return minValueNode(root);
}


void RBTree::updateWaitVar(Node *&ptr)
{
     if (ptr == NULL)
        return;

    updateWaitVar(ptr->left);
    ptr->wait=ptr->wait+1;
    updateWaitVar(ptr->right);
}

void RBTree::updateWait()
{
    updateWaitVar(root);
}

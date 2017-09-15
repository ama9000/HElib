//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_TREE_H
#define SECUREDECISIONTREE_TREE_H

#include "Node.h"

#define TREE_NODE_PRINT_INDENTATION 6

enum Tree_t {COMPLETE, FULL, INCOMPLETE};



class Tree {
private:
    Tree_t  treeType=COMPLETE;
    Node*   root=NULL;
    int     required_depth;

    bool    autoGen= false;

    void printInOrder(Node*, int);
    void printPreOrder(Node*, int);
    void printPostOrder(Node*, int);
    void evaluateInOrder(Node*);
    void evaluatePreOrder(Node*);
    void evaluatePostOrder(Node*);

public:
    /* initialize a tree */
    Tree(Tree_t, int);      // create with the specified depth; node's values are automatically sampled.


    Node* get_RootNode();

    Node* populate_tree(Node*, int);

    void print_tree();
    void evaluate_decision_nodes();

    int get_Current_Depth();

    bool isAutoGen() {
        return this->autoGen;
    }

};



#endif //SECUREDECISIONTREE_TREE_H

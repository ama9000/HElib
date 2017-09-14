//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_TREE_H
#define SECUREDECISIONTREE_TREE_H

#include "Node.h"

enum Tree_t {COMPLETE, FULL, INCOMPLETE};

class Tree {
private:
    Tree_t  treeType=COMPLETE;
    Node*   root=NULL;
    int     current_depth;
    int     required_depth;

    bool    autoGen= false;

    void inOrder(Node* n);
    void preOrder(Node* n);
    void postOrder(Node* n);

public:
    /* initialize a tree */
    Tree(Tree_t, int);      // create with the specified depth; node's values are automatically sampled.


    Node* get_RootNode();

    Node* populate_tree(Node*);

    void print_tree();

    int get_Current_Depth();

    bool isAutoGen() {
        return this->autoGen;
    }

};



#endif //SECUREDECISIONTREE_TREE_H

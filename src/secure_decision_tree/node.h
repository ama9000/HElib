//
// Created by ph on 9/13/17.
//

#ifndef SECUREDECISIONTREE_NODE_H
#define SECUREDECISIONTREE_NODE_H

#include <NTL/ZZX.h>
#include <FHE.h>



using namespace std;
using namespace NTL;

enum Node_t {leaf, decision, dummy};

class node {
private:
    int     ptxt_threshold;
    Ctxt    ctxt_threshold;
    int     ptxt_input_value;
    Ctxt    ctxt_input_value;

    bool    autoGen=false;
    Node_t  nodeType;
    node*   parent=NULL;
    node*   left_child=NULL;
    node*   right_child=NULL;


public:

    node(Node_t, bool);
    node(Node_t , int, int);
    virtual ~node();


    /* Getters and Setters */
    void set_ptxt_threshold(int threshold) {
        this->ptxt_threshold = threshold;
    }
    int get_ptxt_threshold() {
        return this->ptxt_threshold;
    }

    Ctxt get_ctxt_threshold() {
        return this->ctxt_threshold;
    }

    void set_ptxt_input_value(int input_value) {
        this->ptxt_input_value = input_value;
    }
    int get_ptxt_input_value() {
        return this->ptxt_input_value;
    }

    Ctxt get_ctxt_input_value() {
        return this->ctxt_input_value;
    }

    bool isAutoGen() {
        return this->autoGen;
    }

    Node_t get_NodeType() {
        return this->nodeType;
    }

    void set_parent(node* parent) {
        this->parent = parent;
    }
    node* get_parent() {
        return this->parent;
    }

    void set_left_child(node* left_child) {
        this->left_child = left_child;
    }
    node* get_left_child() {
        return this->left_child;
    }

    void set_right_child(node* right_child) {
        this->right_child = right_child;
    }
    node* get_right_child() {
        return this->right_child;
    }







};


#endif //SECUREDECISIONTREE_NODE_H

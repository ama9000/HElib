//
// Created by ph on 9/13/17.
//

#ifndef SECUREDECISIONTREE_NODE_H
#define SECUREDECISIONTREE_NODE_H

#include <NTL/ZZX.h>
#include <FHE.h>


using namespace std;
using namespace NTL;

#define RAND_NODE_VALUE_LOWER_BOUND 1
#define RAND_NODE_VALUE_UPPER_BOUND 15


enum Node_t {ROOT, LEAF, DECISION, DUMMY};

class Node {
private:
    int     ptxt_threshold;
    Ctxt    *ctxt_threshold;
    int     ptxt_input_value;
    Ctxt    *ctxt_input_value;

    bool    autoGen=false;
    Node_t  nodeType;
    Node    *parent=NULL;
    Node    *left_child=NULL;
    Node    *right_child=NULL;


public:
    Node(Node_t, bool);
    Node(Node_t , int, int);
    virtual ~Node();

    void encrypt_node_values();         // encrypt node values
    void evaluate();                    // evaluate the boolean function


    void print_node_values();








    /* Getters and Setters */
    void set_ptxt_threshold(int threshold) {
        this->ptxt_threshold = threshold;
    }
    int get_ptxt_threshold() {
        return this->ptxt_threshold;
    }

    Ctxt *get_ctxt_threshold() {
        return this->ctxt_threshold;
    }

    void set_ptxt_input_value(int input_value) {
        this->ptxt_input_value = input_value;
    }
    int get_ptxt_input_value() {
        return this->ptxt_input_value;
    }

    Ctxt *get_ctxt_input_value() {
        return this->ctxt_input_value;
    }

    bool isAutoGen() {
        return this->autoGen;
    }

    Node_t get_NodeType() {
        return this->nodeType;
    }

    void set_parent(Node* parent) {
        this->parent = parent;
    }
    Node* get_parent() {
        return this->parent;
    }

    void set_left_child(Node* left_child) {
        this->left_child = left_child;
    }
    Node* get_left_child() {
        return this->left_child;
    }

    void set_right_child(Node* right_child) {
        this->right_child = right_child;
    }
    Node* get_right_child() {
        return this->right_child;
    }


};


#endif //SECUREDECISIONTREE_NODE_H

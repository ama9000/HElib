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
    Ctxt*   ctxt_threshold;
    int     ptxt_input_value;
    Ctxt*   ctxt_input_value;

    int     ptxt_leaf_value;
    Ctxt*   ctxt_leaf_value;

    bool    autoGen=false;
    int     nodeIdx;
    int     nodeLevel;
    Node_t  nodeType=DECISION;
    Node*   parent=NULL;
    Node*   left_child=NULL;
    Node*   right_child=NULL;


public:
    Node(Node_t, bool);
    Node(Node_t, int);
    Node(Node_t, Node*, Node*);         // Node type, left child, right child

    virtual ~Node();

    void encrypt_node_values();         // encrypt node values
    void evaluate();                    // evaluate the boolean function


    void print_node_values(int);








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

    int get_NodeIdx() {
        return this->nodeIdx;
    }
    void set_NodeLevel(int level){
        this->nodeLevel=level;
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

    string convert_NodeTypeToString(Node_t nodeType){
        switch(nodeType){
            case ROOT:
                return "ROOT";
            case LEAF:
                return "LEAF";
            case DECISION:
                return "DECISION";
            case DUMMY:
                return "DUMMY";
        }
    }

};


#endif //SECUREDECISIONTREE_NODE_H

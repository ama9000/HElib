//
// Created by ph on 9/13/17.
//

#include "Node.h"
#include "util.h"

using namespace std;
using namespace NTL;


Node::Node(Node_t nodeType, bool autoGen) {
    this->nodeType = nodeType;
    this->autoGen = autoGen;

    if(autoGen) {
        this->ptxt_threshold = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
        this->ptxt_input_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
    }
}

Node::Node(Node_t nodeType, int ptxt_threshold){
    Node(nodeType, false);
    this->ptxt_threshold = ptxt_threshold;
}

Node::Node(Node_t nodeType, Node* left_child, Node* right_child) {
    Node(nodeType, true);
    this->left_child=left_child;
    this->right_child=right_child;
}

Node::~Node() {
    // deallocate memory
}


void Node::evaluate() {
    // perform evaluation of the boolean function
}


void Node::print_node_values() {
    Helper::printInt("threshold", this->ptxt_threshold);
    Helper::printInt("input_value", this->ptxt_input_value);
}



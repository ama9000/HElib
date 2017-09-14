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

Node::Node(Node_t nodeType, int ptxt_threshold, int ptxt_input_value){
    Node(nodeType, false);
    this->ptxt_threshold = ptxt_threshold;
    this->ptxt_input_value = ptxt_input_value;
}

Node::~Node() {
    // deallocate memory
}


void Node::evaluate() {
    // perform evaluation of the boolean function
}


void Node::print_node_values() {

//    helper::debugMSG(this->ptxt_threshold);
//    helper::
}



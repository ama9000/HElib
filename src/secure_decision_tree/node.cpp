//
// Created by ph on 9/13/17.
//

#include "node.h"
#include "util.h"

using namespace std;
using namespace NTL;


node::node(Node_t nodeType, bool autoGen) {
    this->nodeType = nodeType;
    this->autoGen = autoGen;

    if(autoGen) {
        this->ptxt_threshold = helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
        this->ptxt_input_value = helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
    }
}

node::node(Node_t nodeType, int ptxt_threshold, int ptxt_input_value){
    node(nodeType, false);
    this->ptxt_threshold = ptxt_threshold;
    this->ptxt_input_value = ptxt_input_value;
}

node::~node() {
    // deallocate memory
}


void node::evaluate() {
    // perform evaluation of the boolean function
}


void node::print_node_values() {

//    helper::debugMSG(this->ptxt_threshold);
//    helper::
}



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
    this->nodeType = nodeType;
    this->autoGen = false;
    this->ptxt_threshold = ptxt_threshold;
}

/* be careful about constructor delegation */
Node::Node(Node_t nodeType, Node* left_child, Node* right_child){
    this->nodeType = nodeType;
    this->autoGen = true;

    if(autoGen) {
        this->ptxt_threshold = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
        this->ptxt_input_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
    }

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
    cout<<endl;
    Helper::printInt("NodeLevel", this->nodeLevel);
    Helper::printString("NodeType", this->convert_NodeTypeToString(this->nodeType));
    Helper::printInt("threshold", this->ptxt_threshold);
    Helper::printInt("input_value", this->ptxt_input_value);

}



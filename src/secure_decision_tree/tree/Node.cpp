//
// Created by ph on 9/13/17.
//

#include "Node.h"
#include "../common/util.h"
#include <iomanip>



using namespace std;
using namespace NTL;


Node::Node(Node_t nodeType, bool autoGen) {
    this->nodeType = nodeType;
    this->autoGen = autoGen;

    if(this->autoGen) {
        switch (this->nodeType) {
            case ROOT:
            case DECISION:
                this->ptxt_threshold = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                this->ptxt_input_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                break;
            case LEAF:
                this->ptxt_leaf_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                break;
            case DUMMY:
            default:
                break;
        }
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

    if(this->autoGen) {
        switch (this->nodeType) {
            case ROOT:
            case DECISION:
                this->ptxt_threshold = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                this->ptxt_input_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                break;
            case LEAF:
                this->ptxt_leaf_value = Helper::getRandomInt(RAND_NODE_VALUE_LOWER_BOUND, RAND_NODE_VALUE_UPPER_BOUND);
                break;
            case DUMMY:
            default:
                break;
        }
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


void Node::print_node_values(int indent=0) {
    if (indent) {
        cout << setw(indent) << ' ';
    }

    cout << "(lvl: " << this->nodeLevel
         << ", type: " << this->convert_NodeTypeToString(this->nodeType);

    switch (this->nodeType){
        case ROOT:
        case DECISION:
                cout << ", threshold:" << this->ptxt_threshold
                 << ", input:" << this->ptxt_input_value << endl;
            break;
        case LEAF:
                cout << ", leaf_val:" << this->ptxt_leaf_value << endl;
            break;
        case DUMMY:
        default:
            break;
    }
}



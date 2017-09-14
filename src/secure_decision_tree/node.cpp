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

}

node::node(Node_t nodeType, int ptxt_threshold, int ptxt_input_value){
    node(nodeType, false);
    this->ptxt_threshold = ptxt_threshold;
    this->ptxt_input_value = ptxt_input_value;
}

node::~node() {

}



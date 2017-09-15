//
// Created by ph on 9/14/17.
//


#include "util.h"
#include "Tree.h"


#define AUTOGEN true            // whether automatically generating a tree with node values randomly sampled.







int main() {
    cout << endl;
    cout << "Program Started!!" << endl;
    cout << endl;

    //cout << Helper::getRandomInt(0,15) << endl;

//    Node* roo = new Node(ROOT, true);
//    Node_t nt = roo->get_NodeType();
//    Helper::printString("nodeType", roo->convert_NodeTypeToString(nt));



    //cout << "test" << endl;
    Tree* tree = new Tree(COMPLETE, 2);

//    Node* root = tree->get_RootNode();
//    cout << "testing" << endl;
//    Node_t nodeType = root->get_NodeType();
//    cout << "testing2" << endl;
//    Helper::printString("nodeType", root->convert_NodeTypeToString(nodeType));

    tree->print_tree();







}
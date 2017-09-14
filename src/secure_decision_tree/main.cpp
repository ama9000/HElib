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

    //cout << "test" << endl;
    Tree* tree = new Tree(COMPLETE, 2);
    cout << tree->get_Current_Depth() << endl;
    tree->print_tree();







}
//
// Created by ph on 9/14/17.
//

#include "Tree.h"


Tree::Tree(Tree_t treeType, int depth) {
    this->autoGen=true;
    this->required_depth=depth;
    this->treeType=treeType;
    this->current_depth=0;
    populate_tree(this->root);

}


Node* Tree::get_RootNode() {
    return this->root;
}


Node* Tree::populate_tree(Node* node) {
    if(current_depth==required_depth){
        return new Node(LEAF, NULL, NULL);
    } else {
        current_depth++;
        return new Node(DECISION,
                        populate_tree(node->get_left_child()),
                        populate_tree(node->get_right_child())
        );
    }
}


void Tree::print_tree(Node* node) {

}

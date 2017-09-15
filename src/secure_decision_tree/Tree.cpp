//
// Created by ph on 9/14/17.
//

#include "Tree.h"
#include "util.h"


Tree::Tree(Tree_t treeType, int required_depth) {
    this->autoGen=true;
    this->required_depth=required_depth;
    this->treeType=treeType;
    this->root=populate_tree(this->root, 0);

}


Node* Tree::get_RootNode() {
    return this->root;
}


Node* Tree::populate_tree(Node* node, int current_depth) {
    if(current_depth==required_depth){
        node = new Node(LEAF, NULL, NULL);
        node->set_NodeLevel(current_depth+1);
    } else {
        current_depth++;
        node = new Node(DECISION, NULL, NULL);
        node->set_NodeLevel(current_depth);
        node->set_left_child(populate_tree(node->get_left_child(), current_depth));
        node->set_right_child(populate_tree(node->get_right_child(), current_depth));
    }
    return node;
}

void Tree::inOrder(Node* node){
    if(node->get_NodeType()==DECISION){
        inOrder(node->get_left_child());
        node->evaluate();
        node->print_node_values();
        inOrder(node->get_right_child());
    } else if(node->get_NodeType()==LEAF){
        node->print_node_values();
    }
}

//Traverses the tree in a "pre-order" DFS and evaluates the nodes accordingly
void Tree::preOrder(Node* node){
    if(node->get_NodeType()==DECISION){
        node->evaluate();
        node->print_node_values();
        preOrder(node->get_left_child());
        preOrder(node->get_right_child());
    } else if (node->get_NodeType()==LEAF) {
        node->print_node_values();
    }
}

//Traverses the tree in a "post order" DFS and evaluates the nodes accordingly
void Tree::postOrder(Node* node) {
    if (node->get_NodeType() == DECISION) {
        postOrder(node->get_left_child());
        postOrder(node->get_right_child());
        node->evaluate();
        node->print_node_values();
    } else if (node->get_NodeType()==LEAF) {
        node->print_node_values();
    }
}

void Tree::print_tree() {
    preOrder(this->root);
}
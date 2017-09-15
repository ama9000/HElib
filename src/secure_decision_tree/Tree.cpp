//
// Created by ph on 9/14/17.
//

#include "Tree.h"


Tree::Tree(Tree_t treeType, int required_depth) {
    this->autoGen=true;
    this->required_depth=required_depth;
    this->treeType=treeType;
    this->current_depth=0;
    this->root=populate_tree(this->root);

}


Node* Tree::get_RootNode() {
    return this->root;
}


Node* Tree::populate_tree(Node* node) {
    cout << current_depth << endl;
    if(current_depth==required_depth){
        cout << "leaf" << endl;
        return new Node(LEAF, NULL, NULL);
    } else {
        current_depth++;
        Node* new_node = new Node(DECISION, NULL, NULL);
        new_node->set_left_child(populate_tree(new_node->get_left_child()));
        new_node->set_right_child(populate_tree(new_node->get_right_child()));
        return new_node;
    }
}

int Tree::get_Current_Depth(){
    return this->current_depth;
}

void Tree::inOrder(Node* node){
    cout << "test1" << endl;
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
        //node->print_node_values();
        cout << "DECISION" << endl;
        preOrder(node->get_left_child());
        preOrder(node->get_right_child());
    } else if (node->get_NodeType()==LEAF) {
        //node->print_node_values();
        cout << "LEAF" << endl;
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
    //inOrder(this->root);
    preOrder(this->root);
}

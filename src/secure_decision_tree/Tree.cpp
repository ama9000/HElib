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
        Node* new_node = new Node(DECISION, NULL, NULL);
        new_node->set_left_child(populate_tree(new_node->get_left_child()));
        new_node->set_right_child(populate_tree(new_node->get_right_child()));
    }
}

int Tree::get_Current_Depth(){
    return this->current_depth;
}

void Tree::inOrder(Node* node){
    //cout<<node->get_node_type_string(node->get_NodeType())<<endl;
    if(node->get_NodeType()==DECISION){
        inOrder(node->get_left_child());
        //node->evaluate();
        //node->print_node_values();
        cout<<"decision"<<endl;
        inOrder(node->get_right_child());
    }else if(node->get_NodeType()==LEAF){
        cout<<"leaf"<<endl;
        //node->print_node_values();
    }
}

//Traverses the tree in a "pre-order" DFS and evaluates the nodes accordingly
void Tree::preOrder(Node* node){
    if(node->get_NodeType()==DECISION){
        node->evaluate();
        node->print_node_values();
        preOrder(node->get_left_child());
        preOrder(node->get_right_child());
    }
}

//Traverses the tree in a "post order" DFS and evaluates the nodes accordingly
void Tree::postOrder(Node* node) {
    if (node->get_NodeType() == DECISION) {
        postOrder(node->get_left_child());
        postOrder(node->get_right_child());
        node->evaluate();
        node->print_node_values();
    }
}

void Tree::print_tree() {
    inOrder(root);
}

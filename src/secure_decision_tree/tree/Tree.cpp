//
// Created by ph on 9/14/17.
//

#include "Tree.h"
#include "../common/util.h"


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



void Tree::printInOrder(Node* node, int indent=0){
    if(node->get_NodeType()==DECISION){
        printInOrder(node->get_left_child(), indent+TREE_NODE_PRINT_INDENTATION);
        node->print_node_values(indent);
        printInOrder(node->get_right_child(), indent+TREE_NODE_PRINT_INDENTATION);
    } else if(node->get_NodeType()==LEAF){
        node->print_node_values(indent);
    }
}

//Traverses the tree in a "pre-order" DFS and evaluates the nodes accordingly
void Tree::printPreOrder(Node* node, int indent=0){
    if(node->get_NodeType()==DECISION){
        node->print_node_values(indent);
        printPreOrder(node->get_left_child(), indent+TREE_NODE_PRINT_INDENTATION);
        printPreOrder(node->get_right_child(), indent+TREE_NODE_PRINT_INDENTATION);
    } else if (node->get_NodeType()==LEAF) {
        node->print_node_values(indent);
    }
}

//Traverses the tree in a "post order" DFS and evaluates the nodes accordingly
void Tree::printPostOrder(Node* node, int indent=0) {
    if (node->get_NodeType() == DECISION) {
        printPostOrder(node->get_left_child(), indent+TREE_NODE_PRINT_INDENTATION);
        printPostOrder(node->get_right_child(), indent+TREE_NODE_PRINT_INDENTATION);
        node->print_node_values(indent);
    } else if (node->get_NodeType()==LEAF) {
        node->print_node_values(indent);
    }
}

void Tree::print_tree() {
    printPreOrder(this->root);
}


void Tree::evaluateInOrder(Node* node){
    if(node->get_NodeType()==DECISION){
        evaluateInOrder(node->get_left_child());
        node->evaluate();
        evaluateInOrder(node->get_right_child());
    } else if(node->get_NodeType()==LEAF){
        // do nothing
    }
}

//Traverses the tree in a "pre-order" DFS and evaluates the nodes accordingly
void Tree::evaluatePreOrder(Node* node){
    if(node->get_NodeType()==DECISION){
        node->evaluate();
        evaluatePreOrder(node->get_left_child());
        evaluatePreOrder(node->get_right_child());
    } else if (node->get_NodeType()==LEAF) {
        // do nothing
    }
}

//Traverses the tree in a "post order" DFS and evaluates the nodes accordingly
void Tree::evaluatePostOrder(Node* node) {
    if (node->get_NodeType() == DECISION) {
        evaluatePostOrder(node->get_left_child());
        evaluatePostOrder(node->get_right_child());
        node->evaluate();
    } else if (node->get_NodeType()==LEAF) {
        // do nothing
    }
}


void Tree::evaluate_decision_nodes() {
    evaluatePreOrder(this->root);
}
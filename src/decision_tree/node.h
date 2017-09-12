/*
 * node.h
 *
 *      Author: asma
 */

#ifndef LNV2011_TREEEVAL_NODE_H_
#define LNV2011_TREEEVAL_NODE_H_

#include <NTL/ZZX.h>
#include "../HEctxt.h"
#include <vector>

using namespace std;
using namespace NTL;

// Class node defines the structure of a single node in the decision tree
class node {

private:
	int value; 						// Stores the value of a node
	ZZX valueX;					// Stores the polynomial representation of the node value
    HEctxt *vctxt;					// Stores the encrypted version of the value
    Ctxt pathCost;					// Stores the encrypted path cost to this node
	int index;						// Stores the class label for leaf nodes. For non-leaf nodes, it stores the value of the attribute of the parent's' split
	bool isLeaf;					// boolean flag for leaf nodes
    bool isRight;                   // boolean flag to distinguish if leaf is right child of parent node
	node* left;						// Stores a pointer to the node's left child
	node* right;					// Stores a pointer to the node's right child

public:
	node();
	node(int, int, bool, node*, node*);
	virtual ~node();


	// GET ELEMENETS:
	int get_value(){ return value; }
	ZZX get_valueX(){
		return valueX;
	}
	HEctxt* get_vctxt(){
		return vctxt;
	}
	int get_index(){
		return index;
	}
	bool get_isLeaf(){
		return isLeaf;
	}
    bool get_isRight(){
        return isRight;
    }
	node* get_left(){
		return left;
	}
	node* get_right(){
		return right;
	}

	// SET ELEMENTS:
	void set_value(int v){
		value = v;
	}
	void set_valueX(ZZX v){
		valueX = v;
	}
	void set_vctxt(HEctxt *vc){
		vctxt = vc;
	}
	void set_index(int id){
		index = id;
	}
	void set_isLeaf(bool t){
		isLeaf = t;
	}
    void set_isRight(bool ir){
        isRight = ir;
    }
	void set_left(node* l){
		left = l;
	}
	void set_right(node* r){
		right = r;
	}

};


#endif

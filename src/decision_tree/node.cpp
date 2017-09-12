/*
 * node.cpp
 *
 *      Author: asma
 */

#include "node.h"

using namespace std;

node::node() {
	value = 0;
	vctxt = new HEctxt();
	index = 0;
	isLeaf = false;
    isRight = false;
	left = NULL;
	right = NULL;
}

node::node(int v, int id, bool leaf, bool ir, node* l, node* r){
	value = v;
	vctxt = new HEctxt();
	index = id;
	isLeaf = leaf;
    isRight = ir;
	left = l;
	right = r;
}

node::~node() {
	// TODO Auto-generated destructor stub
}


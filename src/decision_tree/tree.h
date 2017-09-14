/*
 * TreeEval.h
 *
 *      Author: asma
 */

#ifndef LNV2011_TREEEVAL_TREEEVAL_H_
#define LNV2011_TREEEVAL_TREEEVAL_H_

#include <NTL/ZZX.h>
//#include "../HEctxt.h"
#include <vector>
//#include "../HEcrypto.h"
//#include "../HEkeygen.h"
//#include "../HEpubkey.h"
//#include <common/timing.h>
//#include "node.h"

using namespace std;
using namespace NTL;

// Class node defines the structure of a single node in the decision tree
class node{

	//public FHEPubKey;

private:
	int value; 								// Stores the value of a node
	ZZX valueX;								// Stores the polynomial representation of the node value
	Ctxt vctxt = Ctxt(*publicKey);			// Stores the encrypted version of the value
	Ctxt path_cost = Ctxt(*publicKey);		// Stores the encrypted path cost to this node
	int index;								// Stores the class label for leaf nodes. For non-leaf nodes, it stores the value of the attribute of the parent's' split
	bool isLeaf;								// boolean flag for leaf nodes
	bool isRight;                   			// boolean flag to distinguish if leaf is right child of parent node
	node* parent;							// Stores a pointer to the node's parent
	node* left;								// Stores a pointer to the node's left child
	node* right;								// Stores a pointer to the node's right child

public:
	node();
	node(int, int, bool, bool, node*, node*, node*);
	virtual ~node();

	// GET ELEMENETS:
	int get_value(){ return value; }
	ZZX get_valueX(){
		return valueX;
	}
	Ctxt get_vctxt(){
		return vctxt;
	}
	Ctxt get_pathCost(){
		return path_cost;
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
	node* get_parent(){
		return parent;
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
	void set_vctxt(Ctxt vc){
		vctxt = vc;
	}
	void set_pathCost(Ctxt pc){
		path_cost = pc;
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
	void set_parent(node* p){
		parent = p;
	}
	void set_left(node* l){
		left = l;
	}
	void set_right(node* r){
		right = r;
	}

};


vector<node> StructTree(int height, string type, FHEPubKey* publicKey);

//int EvaluateTreePlain(node* n);
//HEctxt *EvaluateTree(node* n, bool he, HEctxt *one, HEcrypto *he_crypto, vector<HEpubkey *> he_homomorphismKeys);
//
//void test_tree_plaintxt(int height, string type);
//void run_HEtree_test(int height, string type, LNV2011Params *params, HEcrypto* he_crypto, HEkeygen *he_keygen, HEpubkey *he_pubkey, const ZZ_pX sk, vector<HEpubkey *> he_homomorphismKeys, timing *timer);
//void run_tree_example(); //initial test
#endif /* LNV2011_TREEEVAL_TREEEVAL_H_ */

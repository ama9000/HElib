/*
 * tree.cpp
 *
 *      Author: asma
 */


#include "decision_tree/tree.h"
#include <iostream>
#include <fstream>

#include <string>
#include <math.h>
#include <NTL/ZZX.h>
#include <random>

//#include <common/libntl_addon.h>
//#include <common/JSONParser.h>

using namespace std;
using namespace NTL;

node::node() {
    value = 0;
    //vctxt = new HEctxt();
    index = 0;
    isLeaf = false;
    isRight = false;
    left = NULL;
    right = NULL;
}

node::node(int v, int id, bool leaf, bool ir, node* l, node* r){
    value = v;
    //vctxt = new HEctxt();
    index = id;
    isLeaf = leaf;
    isRight = ir;
    left = l;
    right = r;
}

node::~node() {
    // TODO Auto-generated destructor stub
}

/* =========   Tree Structure and Evaluation  ========= */

// Structure:
vector<node> StructTree(int height, string type ) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<long> dis(0, 1);

	vector<node> Nodes;
	int index = 0;

	if (type == "perfect") {
		// A special case where each one of the internal nodes has two children at all levels

		// Step 1: Generate nodes with a random values:
		int nodeNum = pow(2,height+1)-1;	// 2^(height+1) - 1;
		int leafNum = pow(2,height);		//2^height;

		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, NULL, NULL);
			Nodes.push_back(n);
		}

		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);
		Nodes[0].set_right(&Nodes[2]);

		int stop = nodeNum-leafNum;

		// Step 3: Set internal nodes' children:
		int j = 2;
		for (int i = 1; i < stop; i++){
			Nodes[i].set_left(&Nodes[i+j]);
			Nodes[i].set_right(&Nodes[i+j+1]);
			j++;
		}

		// Step 4: Set leaf flags
		int value = 1;
		for (int i=0; i<nodeNum; i++) {
			if (Nodes[i].get_left() == NULL && Nodes[i].get_right() == NULL){
				Nodes[i].set_isLeaf(true); //= true;
				Nodes[i].set_value(value++); // = value++;
			}
		}
		// Printing Tree Information:
		cout << endl;
		cout << "Tree Height = " << height << endl;
		cout << "Total Number of Nodes = " << nodeNum << endl;
		cout << "Number of Decision Nodes = " << stop << endl;
		cout << "Number of Leaf Nodes = " << leafNum << endl << endl;
	}

	if (type == "full") {
		// A special case where each of the internal nodes has two or 0 children
		//src: http://homepages.math.uic.edu/~leon/cs-mcs401-s08/handouts/nearly_complete.pdf

		int remLeaf = pow(2,height) / 2;
		int nodeNum = (pow(2,height+1)-1) - remLeaf;

		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, NULL, NULL);
			Nodes.push_back(n);
		}

		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);
		Nodes[0].set_right(&Nodes[2]);

		// Step 3: Set internal nodes' children:
		int stop = nodeNum;
		for (int i = 1; i < stop; i++){
			if ( 2*i+1 < stop) {
				Nodes[i].set_left(&Nodes[2*i+1]);       //odd index
			}
			if ( (2*i)+2 < stop) {
				Nodes[i].set_right(&Nodes[2*i+2]);     // even index
			}
		}

		// Step 4: Set leaf flags
		int value = 1;
		for (int i=0; i<nodeNum; i++) {
			if (Nodes[i].get_left() == NULL && Nodes[i].get_right() == NULL){
				Nodes[i].set_isLeaf(true); //= true;
				Nodes[i].set_value(value++);
			}
		}
		// Printing Tree Information:
		value = value-1;
		int decNodes = nodeNum-value;
		cout << endl;
		cout << "Tree Height = " << height << endl;
		cout << "Total Number of Nodes = " << nodeNum << endl;
		cout << "Number of Decision Nodes = " << decNodes << endl;
		cout << "Number of Leaf Nodes = " << value << endl << endl;
	}

	return Nodes;
}

//void test_tree_plaintxt(int height, string type) {
//
//	vector<node> T = StructTree(height, type);
//
//	cout << "Tree Height = " << height << endl << endl;
//
//	for (int i=0; i<T.size(); i++){
//		cout << T[i].get_index() << ": " << T[i].get_value();
//		if (!T[i].get_isLeaf()){
//			if (T[i].get_left() != NULL ){
//				cout << " ========= Left: " << T[i].get_left()->get_index();
//			}
//			if (T[i].get_right() != NULL ){
//				cout << " | Right: " << T[i].get_right()->get_index();;
//			}
//		} else {
//			cout << " ========= Leaf Node!!";
//		}
//
//		cout << " newValue: " << T[i].get_value();
//		cout << endl;
//	}
//
//	int res = EvaluateTreePlain(&T[0]);
//
//	cout << "Plaintext Result= " << res;
//
//	cout << endl;
//
//}

//void run_HEtree_test(int height, string type, LNV2011Params *params, HEcrypto* he_crypto, HEkeygen *he_keygen, HEpubkey *he_pubkey, const ZZ_pX sk, vector<HEpubkey *> he_homomorphismKeys, timing *timer) {
//
//	// Set the encrypted value of 1:
//	ZZ_pX one_x = ZZ_pX();
//	SetCoeff(one_x, 0);
//	HEctxt *one = he_crypto->encrypt(one_x);
//
//	// Build a tree of the specified height:
//	timer->start();
//	vector<node> T = StructTree(height, type);
//	timer->stop("Tree_creation", false);
//
//	// Set ZZ_pX and HEctxt values for each node:
//	char* bin;
//	ZZ_pX valX;
//	HEctxt *Valctxt;
//	for (int i=0; i<T.size(); i++) {
//		bin = toBinary(T[i].get_value());
//		valX = createPolyFromCoeffsArray(bin);
//		modT(valX, params->t);
//		T[i].set_valueX(valX);
//		Valctxt = he_crypto->encrypt(valX);
//		T[i].set_vctxt(Valctxt);
//	}

	// Printing Tree:
	//cout << "Tree Height = " << height << endl << endl;

//	for (int i=0; i<T.size(); i++){
//		cout << T[i].get_index() << ": " << T[i].get_value() << " : ";
//		printZZ_pX(T[i].get_valueX());
//		if (!T[i].get_isLeaf()){
//			if (!T[i].get_isLeaf()){
//				if (T[i].get_left() != NULL ){
//					cout << " ========= Left: " << T[i].get_left()->get_index();
//				}
//				if (T[i].get_right() != NULL ){
//					cout << " | Right: " << T[i].get_right()->get_index();;
//				}
//			} else {
//				cout << " ========= Leaf Node!!";
//			}
//			cout << " newValue: " << T[i].get_value();
//			cout << endl;
//		}
//	}
	//vector<HEpubkey *> he_homomorphismKeys = he_keygen->createHomomorphismKeys(sk);

	// Test tree evaluation in plaintext:
//	timer->start();
//	int res = EvaluateTreePlain(&T[0]);
//	timer->stop("Plain_tree_eval", false);
//
//	// Test tree evaluation under encryption:
//	timer->start();
//	HEctxt *he_res = EvaluateTree(&T[0], true, one, he_crypto, he_homomorphismKeys);
//	timer->stop("HE_tree_eval", false);
//
//	ZZ_pX he_res_dec;
//	timer->start();
//	he_crypto->decrypt(he_res_dec, he_res, sk);
//	timer->stop("HE_tree_result_dec", false);
//
//	char *bin_res = toBinary(res);
//	ZZ_pX res_X = createPolyFromCoeffsArray(bin_res);
//
//	cout << "Plaintext Result = " << res << " = ";
//	printZZ_pX(res_X);
//
//	cout << endl;
//
//	cout << "HE Result = ";
//	printZZ_pX(he_res_dec);
//
//	cout << endl;
//}
//
//
//void run_tree_example() {
//	cout << "Evaluation of Encrypted Decision Tree" << endl;}
//



/*
 * tree.cpp
 *
 *      Author: asma
 */


#include "decision_tree/tree.h"

#include <iostream>
#include <fstream>

#include<FHE.h>

#include<vector>
#include <string>

#include <math.h>
#include <NTL/ZZX.h>

#include <random>

using namespace std;
using namespace NTL;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<long> dis(0, 15);

// /* =========   Tree Structure  ========= */

node::node() {
	value = 0;
	index = 0;
	isLeaf = false;
	isRight = false;
	left = NULL;
	right = NULL;
}

node::node(int v, int id, bool leaf, bool ir, node* p, node* l, node* r){
	value = v;
	index = id;
	isLeaf = leaf;
	isRight = ir;
	parent = p;
	left = l;
	right = r;
}

node::~node() {
	// TODO Auto-generated destructor stub
}

// Structure: TODO: set isRight flags!!
vector<node> StructTree(int height, string type ) {

	vector<node> Nodes;
	int index = 0;

	if (type == "perfect") {
		// A special case where each one of the internal nodes has two children at all levels

		// Step 1: Generate nodes with a random values:
		int nodeNum = pow(2,height+1)-1;	// 2^(height+1) - 1;
		int leafNum = pow(2,height);		//2^height;

		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, false, NULL, NULL, NULL);
			Nodes.push_back(n);
		}

		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);
		Nodes[1].set_parent(&Nodes[0]);
		Nodes[0].set_right(&Nodes[2]);
		Nodes[2].set_parent(&Nodes[0]);
		Nodes[2].set_isRight(true);

		int stop = nodeNum-leafNum;

		// Step 3: Set internal nodes' children and parents:
		int j = 2;
		for (int i = 1; i < stop; i++){
			Nodes[i].set_left(&Nodes[i+j]);
			Nodes[i+j].set_parent(&Nodes[i]);
			Nodes[i].set_right(&Nodes[i+j+1]);
			Nodes[i+j+1].set_parent(&Nodes[i]);
			Nodes[i+j+1].set_isRight(true);
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
		cout << "# Printing Created Tree Information #" << endl;
		cout << "Tree Height = " << height << endl;
		cout << "Total Number of Nodes = " << nodeNum << endl;
		cout << "Number of Decision Nodes = " << stop << endl;
		cout << "Number of Leaf Nodes = " << leafNum << endl << endl;

		// Printing Node's int values + Setting isRight flag

		cout << "# Printing Created Tree Nodes #" << endl;
		for (int i=0; i<nodeNum; i++){
			cout << "Node index= " << Nodes[i].get_index() << ", Value= " <<  Nodes[i].get_value();
			if (Nodes[i].get_isRight()){
				cout << " Right Child !! ";
			}
			if (Nodes[i].get_isLeaf())
				cout << " Leaf Node !! ";
			cout << endl;
		}
		cout << endl;
		cout << endl;
	}

	if (type == "full") {
		// A special case where each of the internal nodes has two or 0 children
		//src: http://homepages.math.uic.edu/~leon/cs-mcs401-s08/handouts/nearly_complete.pdf

		int remLeaf = pow(2,height) / 2;
		int nodeNum = (pow(2,height+1)-1) - remLeaf;

		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, false, NULL, NULL, NULL);
			Nodes.push_back(n);
		}

		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);
		Nodes[1].set_parent(&Nodes[0]);
		Nodes[0].set_right(&Nodes[2]);
		Nodes[2].set_parent(&Nodes[0]);
		Nodes[2].set_isRight(true);

		// Step 3: Set internal nodes' children:
		int stop = nodeNum;
		for (int i = 1; i < stop; i++){
			if ( 2*i+1 < stop) {
				Nodes[i].set_left(&Nodes[2*i+1]);       //odd index
				Nodes[2*i+1].set_parent(&Nodes[i]);
			}
			if ( (2*i)+2 < stop) {
				Nodes[i].set_right(&Nodes[2*i+2]);     // even index
				Nodes[2*i+2].set_parent(&Nodes[i]);
				Nodes[2*i+2].set_isRight(true);
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
		cout << "# Printing Created Tree Information #" << endl;
		cout << "Tree Height = " << height << endl;
		cout << "Total Number of Nodes = " << nodeNum << endl;
		cout << "Number of Decision Nodes = " << decNodes << endl;
		cout << "Number of Leaf Nodes = " << value << endl << endl;

		// Printing Node's int values:

		cout << "# Printing Created Tree Nodes #" << endl;
		for (int i=0; i<nodeNum; i++){
			cout << "Node index= " << Nodes[i].get_index() << ", Value= " <<  Nodes[i].get_value();
			if(Nodes[i].get_isRight()){
				cout << " Right Child !! ";
			}
			if (Nodes[i].get_isLeaf())
				cout << " Leaf Node !! ";
			cout << endl;
		}
		cout << endl;
		cout << endl;
	}

	return Nodes;
}



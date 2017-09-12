/*
 * TreeEval.cpp
 *
 *      Author: asma
 */


#include "TreeEval.h"
#include <iostream>
#include <fstream>

#include <string>
#include <math.h>
#include <NTL/ZZ.h>
#include <random>

#include <common/libntl_addon.h>
#include <common/JSONParser.h>

using namespace std;
using namespace NTL;

void printZZXVector(string name, const vector<ZZX> v){ 	//EDITED from util.cpp
	cout<<endl;
	cout<< name << "(" << v.size() << ")" <<endl;
	for(int i=0;i<v.size();i++){
		printZZX(v[i]);
	}
}

/* =========   Binary Transformation  ========= */

char *toBinary(int number) {

	int bit_length;
	// set the length to the minimum number of bits required to represent the integer in binary:
	if (number == 0)
		bit_length = 1;
	else
		bit_length = floor(log2(number)) + 1;

	//cout << "length: " << bit_length << endl; // DEBUG

	//cout << "("<<number<<")_10 = ";

	// define variables:
	int remainder;
	char *binaryBits = new char[bit_length+2]{0};
	binaryBits[0] = '0';
	binaryBits[bit_length] = '0';
	binaryBits[bit_length+1] = '\0';

	int pos = 0;

	while (number > 0) {
		remainder = number%2;
		if (remainder == 0)
			binaryBits[pos] = '0';
		else
			binaryBits[pos] = '1';
		number = number>>1;		//number/2; Using logical shift right
		pos++;
	}

	//cout << "("<<binaryBits<<")_2" << endl;
	//cout << "Bit Length: = "<<strlen(binaryBits)<< endl;
	return binaryBits;
}

char *toBinary(int number, int bit_length) {
	//cout << "length: " << bit_length << endl; // DEBUG
	//cout << "("<<number<<")_10 = ";

	// define variables:
	int remainder;
	char *binaryBits = new char[bit_length+2]{0};
	binaryBits[0] = '0';
	binaryBits[bit_length] = '0';
	binaryBits[bit_length+1] = '\0';

	int pos = 0;

	while (pos < bit_length) {
		binaryBits[pos] = '0';
		pos++;
	}

	pos = 0;

	while (number > 0) {
		remainder = number%2;
		if (remainder == 0)
			binaryBits[pos] = '0';
		else
			binaryBits[pos] = '1';
		number = number>>1;		//number/2; Use logical shift right
		pos++;
	}
	//memset(binaryBits, '\0', 0);

	//cout << "("<<binaryBits<<")_2" << endl;
	return binaryBits;
}

void printBits (char *bits) {
	int len = strlen(bits);
	//cout << "length: " << len << endl;
	for (int i=0; i < len-1; i++) {
		cout<<bits[i];
	}
	cout << endl;
}

char *reverseBinary(char *Binary) {
	int len = strlen(Binary);
	char *reversed = new char[len+1]();
	char temp;
	int pos = 0;
	for (int i=len-1; i >= 0; i--) {
		reversed[pos] = Binary[i];
		pos++;
	}

	return reversed;

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
				Nodes[i].set_left(&Nodes[2*i+1]);
			}
			if ( (2*i)+2 < stop) {
				Nodes[i].set_right(&Nodes[2*i+2]);
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

	if (type == "TBOC") { //Two Branches, One Child TBOC
		// A special case where each of the internal nodes has one child (excluding the root)
		int nodeNum = (2*height) + 1;
		int leafNum = 2;

		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, NULL, NULL);
			Nodes.push_back(n);
		}

		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);
		Nodes[0].set_right(&Nodes[2]);

		int stop = nodeNum-leafNum;
		//cout << endl << nodeNum << "	" << leafNum << "	" << stop << endl;

		// Step 3: Set internal nodes' children:
		for (int i = 1; i < stop; i++){
			if (i%2==0) {
				Nodes[i].set_right(&Nodes[i+2]);
			} else {
				Nodes[i].set_left(&Nodes[i+2]);
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

	}

	if (type == "OBOC") { // One Branch, One Child OBOC
		// A special case where each one of the internal nodes has one child (including the root)

		// Step 1: Generate nodes with a random values:
		int nodeNum = height+1;
		int leafNum = 1;

		cout << endl;
		for (int i=0; i<nodeNum; i++){
			node n (dis(gen), index++, false, NULL, NULL);
			Nodes.push_back(n);
		}
		// Step 2: Set the root's children:
		Nodes[0].set_left(&Nodes[1]);

		int stop = nodeNum-leafNum;

		// Step 3: Set internal nodes' children:
		for (int i = 1; i < stop; i++){
			Nodes[i].set_left(&Nodes[i+1]);
		}

		// Step 4: Set leaf flags
		int value = 1;
		for (int i=0; i<nodeNum; i++) {
			if (Nodes[i].get_left() == NULL && Nodes[i].get_right() == NULL){
				Nodes[i].set_isLeaf(true); //= true;
				Nodes[i].set_value(value++); // = value++;
			}
		}
	}

	return Nodes;
}


// Evaluation
int EvaluateTreePlain(node* n){
	int result;
	// Base case:
	if(n->get_isLeaf()){
		result = n->get_value();
	}
	// node has only left branch/child:
	if (n->get_left() != NULL && n->get_right() == NULL){
		result = (EvaluateTreePlain(n->get_left()));
	}
	// node has only right branch/child:
	if (n->get_left() == NULL && n->get_right() != NULL){
		result = (EvaluateTreePlain(n->get_right()));
	}
	// node has both left and right branches/children:
	if (n->get_left() != NULL && n->get_right() != NULL){
		result = (1-n->get_value())*(EvaluateTreePlain(n->get_left())) +
				(n->get_value())*(EvaluateTreePlain(n->get_right()));
	}
	return result;
}

HEctxt *EvaluateTree(node* n, bool he, HEctxt *one, HEcrypto *he_crypto, vector<HEpubkey *> he_homomorphismKeys){

	HEctxt *result;
	// Base case:
	if(n->get_isLeaf()){
		result = n->get_vctxt();
	}
	// node has only left branch/child:
	if (n->get_left() != NULL && n->get_right() == NULL){
		//HEctxt *one_minus_val = he_crypto->subtraction(n->get_vctxt(),one);
		HEctxt *child = EvaluateTree(n->get_left(), true, one, he_crypto, he_homomorphismKeys);
		//result = he_crypto->multiplication_with_relinearization(one_minus_val, child, he_homomorphismKeys);
		result = child;
	}
	// node has only right branch/child:
	if (n->get_left() == NULL && n->get_right() != NULL){
		//result = (n->get_vctxt())*(EvaluateTree(n->get_right(), true, one, he_homomorphismKeys));
		HEctxt *child = EvaluateTree(n->get_right(), true, one, he_crypto, he_homomorphismKeys);
		//result = he_crypto->multiplication_with_relinearization(n->get_vctxt(), child, he_homomorphismKeys);
		result = child;
	}
	// node has both left and right branches/children:
	if (n->get_left() != NULL && n->get_right() != NULL){
		HEctxt *one_minus_val = he_crypto->subtraction(n->get_vctxt(),one);

		HEctxt *child1 = EvaluateTree(n->get_left(), true, one, he_crypto, he_homomorphismKeys);
		HEctxt *left = he_crypto->multiplication_with_relinearization(one_minus_val, child1, he_homomorphismKeys);

		HEctxt *child2 = EvaluateTree(n->get_right(), true, one, he_crypto, he_homomorphismKeys);
		HEctxt *right = he_crypto->multiplication_with_relinearization(n->get_vctxt(), child2, he_homomorphismKeys);

		result = he_crypto->addition(left,right);

	}
	return result;
}

void test_tree_plaintxt(int height, string type) {

	vector<node> T = StructTree(height, type);

	cout << "Tree Height = " << height << endl << endl;

	for (int i=0; i<T.size(); i++){
		cout << T[i].get_index() << ": " << T[i].get_value();
		if (!T[i].get_isLeaf()){
			if (T[i].get_left() != NULL ){
				cout << " ========= Left: " << T[i].get_left()->get_index();
			}
			if (T[i].get_right() != NULL ){
				cout << " | Right: " << T[i].get_right()->get_index();;
			}
		} else {
			cout << " ========= Leaf Node!!";
		}

		cout << " newValue: " << T[i].get_value();
		cout << endl;
	}

	int res = EvaluateTreePlain(&T[0]);

	cout << "Plaintext Result= " << res;

	cout << endl;

}

void run_HEtree_test(int height, string type, LNV2011Params *params, HEcrypto* he_crypto, HEkeygen *he_keygen, HEpubkey *he_pubkey, const ZZ_pX sk, vector<HEpubkey *> he_homomorphismKeys, timing *timer) {

	// Set the encrypted value of 1:
	ZZ_pX one_x = ZZ_pX();
	SetCoeff(one_x, 0);
	HEctxt *one = he_crypto->encrypt(one_x);

	// Build a tree of the specified height:
	timer->start();
	vector<node> T = StructTree(height, type);
	timer->stop("Tree_creation", false);

	// Set ZZ_pX and HEctxt values for each node:
	char* bin;
	ZZ_pX valX;
	HEctxt *Valctxt;
	for (int i=0; i<T.size(); i++) {
		bin = toBinary(T[i].get_value());
		valX = createPolyFromCoeffsArray(bin);
		modT(valX, params->t);
		T[i].set_valueX(valX);
		Valctxt = he_crypto->encrypt(valX);
		T[i].set_vctxt(Valctxt);
	}

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
	timer->start();
	int res = EvaluateTreePlain(&T[0]);
	timer->stop("Plain_tree_eval", false);

	// Test tree evaluation under encryption:
	timer->start();
	HEctxt *he_res = EvaluateTree(&T[0], true, one, he_crypto, he_homomorphismKeys);
	timer->stop("HE_tree_eval", false);

	ZZ_pX he_res_dec;
	timer->start();
	he_crypto->decrypt(he_res_dec, he_res, sk);
	timer->stop("HE_tree_result_dec", false);

	char *bin_res = toBinary(res);
	ZZ_pX res_X = createPolyFromCoeffsArray(bin_res);

	cout << "Plaintext Result = " << res << " = ";
	printZZ_pX(res_X);

	cout << endl;

	cout << "HE Result = ";
	printZZ_pX(he_res_dec);

	cout << endl;

}


void run_tree_example() {
	cout << "Evaluation of Encrypted Decision Tree" << endl;

	// 1. Set plaintext values:
	//int ab[3] = {1,0,1}; // a, b1, b2
	//int ab[3] = draw_ab(); // a, b1, b2
	//int clss[4] = {1,2,3,4}; // c1, c2, c3, c4

	/*
	 2. Encrypt each class value:
	 */
	// 2.1 create HE crypto system parameters, generate public key:
	// instantiate new Params and jasonParser objects:
	/*
	LNV2011Params params;
	JSONParser *jsonParser = new JSONParser();

	// instantiate a new HEcrypto object
	HEcrypto* he_crypto = new HEcrypto(&params, jsonParser);

	// pass HEcrypto object for accessing f, q, e
	HEkeygen *he_keygen = new HEkeygen(&params, he_crypto, jsonParser);

	// sample s from GaussianNoise
	ZZ_pX param_s;
	while(param_s==0){
		param_s = generatePolyZZXWithCoeffFromGaussianNoise(params.n, params.mean, params.standard_deviation);
	}

	// compute a0, a1
	HEpubkey *he_pubkey = he_keygen->createHEpubkey(param_s);
	he_crypto->addHEpubkey(he_pubkey);


	// 2.2 convert plaintext data elements to their binary representations:
	// c1, c2, c3, c4
	vector<char*> clss_binary_bits;
	for (int i=0; i<4; i++) {
		clss_binary_bits.push_back(toBinary(clss[i]));
		printBits(clss_binary_bits[i]);
	}

	// a, b1, b2
	vector<char*> ab_binary_bits;
	for (int i=0; i<3; i++) {
		ab_binary_bits.push_back(toBinary(ab[i]));
		printBits(ab_binary_bits[i]);
	}

	// 2.3 construct a vector of polynomials from the binary strings:
	// c1, c2, c3, c4
	vector<ZZ_pX> clss_polys_vector;
	for (int i=0; i<4; i++) {
		clss_polys_vector.push_back(createPolyFromCoeffsArray(clss_binary_bits[i]));
		modT(clss_polys_vector[i], params.t);
	}
	printZZ_pXVector("clss_polys_vector", clss_polys_vector);

	// a, b1, b2
	vector<ZZ_pX> ab_polys_vector;
	for (int i=0; i<4; i++) {
		ab_polys_vector.push_back(createPolyFromCoeffsArray(ab_binary_bits[i]));
		modT(ab_polys_vector[i], params.t);
	}
	printZZ_pXVector("ab_polys_vector", ab_polys_vector);

	// 2.4 Apply component-wise encryption operation:
	// c1, c2, c3, c4
	//for (int i = 0; i < 4; i++) {
		vector<HEctxt*> clss_ctxt_vector;
		clss_ctxt_vector = (he_crypto->encrypt_vector(clss_polys_vector, he_pubkey));
	//}

	// a, b1, b2
	//for (int i = 0; i < 3; i++) {
		vector<HEctxt*> ab_ctxt_vector;
		ab_ctxt_vector = (he_crypto->encrypt_vector(ab_polys_vector, he_pubkey));
	//}

	 // 3. Perform Decision Tree Evaluation:
	 //
	// 3.1. Evaluate the Tree Equation:
/*
	// a(b2.c4 + (1-b2).c3) + (1-a)(b1.c2+(1-b1).c1)
	ZZ_pX one_x = ZZ_pX();
	SetCoeff(one_x, 0);

	//(1-a), (1-b1), (1-b2):
	HEctxt *one = he_crypto->encrypt(one_x);
	HEctxt *one_minus_a = he_crypto->subtraction(ab_ctxt_vector[1],one);
	HEctxt *one_minus_b1 = he_crypto->subtraction(ab_ctxt_vector[2],one);
	HEctxt *one_minus_b2 = he_crypto->subtraction(ab_ctxt_vector[3],one);

	// b2.c4, (1-b2).c3), b1.c2, (1-b1).c1
	// create he_homorphism keys:
	vector<HEpubkey *> he_homomorphismKeys = he_keygen->createHomomorphismKeys(param_s);
	// multiplication_with_relinearization(const HEctxt *m_ctxt, const HEctxt *m1_ctxt, const vector<HEpubkey *> he_homorphismKeys)
	HEctxt *b11_times_c1_relin = he_crypto->multiplication_with_relinearization( one_minus_b1, clss_ctxt_vector[0], he_homomorphismKeys);
	HEctxt *b1_times_c2_relin = he_crypto->multiplication_with_relinearization( ab_ctxt_vector[2], clss_ctxt_vector[1], he_homomorphismKeys);
	HEctxt *b21_times_c3_relin = he_crypto->multiplication_with_relinearization( one_minus_b2, clss_ctxt_vector[2], he_homomorphismKeys);
	HEctxt *b2_times_c4_relin = he_crypto->multiplication_with_relinearization( ab_ctxt_vector[2], clss_ctxt_vector[3], he_homomorphismKeys);

	// (b2.c4 + (1-b2).c3), (b1.c2 + (1-b1).c1)
	// addition(const HEctxt *m1_ctxt, const HEctxt *m2_ctxt)
	HEctxt *sum_a_right_side = he_crypto->addition(b2_times_c4_relin,b21_times_c3_relin);
	HEctxt *sum_a_left_side = he_crypto->addition(b1_times_c2_relin,b11_times_c1_relin);

	// result: a(b2.c4 + (1-b2).c3) + (1-a)(b1.c2+(1-b1).c1)
	HEctxt *prod_a_right_side_relin = he_crypto->multiplication_with_relinearization( ab_ctxt_vector[0], sum_a_right_side, he_homomorphismKeys);
	HEctxt *prod_a_left_side_relin = he_crypto->multiplication_with_relinearization( one_minus_a, sum_a_left_side, he_homomorphismKeys);
	HEctxt *tree_result = he_crypto->addition(prod_a_right_side_relin,prod_a_left_side_relin);


	 * 4. Decrypt and Output the Result:

	ZZ_pX result_dec;
	he_crypto->decrypt(result_dec, tree_result, param_s);
	cout << "Result= " << result_dec << endl;

	 */

}




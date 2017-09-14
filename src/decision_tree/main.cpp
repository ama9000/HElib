#include<iostream>
#include<FHE.h>
#include<vector>
#include <string>
#include <math.h>
#include <random>
#include <cstddef>
#include <sys/time.h>

//#include "decision_tree/tree.h"
//#include "decision_tree/util.h"
//#include <decision_tree/timing.h>

using namespace std;
using namespace NTL;

typedef unsigned int uint;
typedef unsigned long ulong;

class Timer		// source: https://github.com/arosharodrigo/HElib/blob/master/helib/src/io/myUtils.h
{
public:
	void start() { m_start = my_clock(); }
	void stop() { m_stop = my_clock(); }
	double elapsed_time() const {
		return m_stop - m_start;
	}

private:
	double m_start, m_stop;
	double my_clock() const {		//return time in seconds
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec + tv.tv_usec * 1e-6;
	}
};

// /* ========= Public Params ========= */

FHEcontext* context;
FHESecKey* secretKey;
FHEPubKey* publicKey;
ZZX G;

Timer timer;

random_device rd;
mt19937 gen(rd());
std::uniform_int_distribution<long> dis(0, 15);

// /* ========= Utility Functions ========= */

ZZX createPolyFromCoeffsArray(char *coeffs){
	ZZX poly;
	for (int i=0; i<strlen(coeffs)-1;i++)
		SetCoeff(poly, i, coeffs[i]-'0');
	SetCoeff(poly, strlen(coeffs)-1, 1);
	return poly;
}

char *toBinary(int number) {

	int bit_length;
	// set the length to the minimum number of bits required to represent the integer in binary:
	if (number == 0)
		bit_length = 1;
	else
		bit_length = floor(log2(number)) + 1;

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
	return binaryBits;
}

char *toBinary(int number, int bit_length) {

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

	return binaryBits;
}

void printBits(char *bits) {
	int len = strlen(bits);
	//cout << "length of binary string: " << len << endl;
	for (int i=0; i < len-1; i++) {
		cout<<bits[i];
	}
	cout << endl;
}

void printZZX(ZZX poly) {
	int len = deg(poly);

	//cout << "length = degree = " << len << endl;
	for (int i=0; i < len+1; i++) {
		cout<<poly[i];
	}
	cout << endl;
}

void setupHElib(long p, long r, long L, long c, long w, long d, long k, long s) {
	long m = FindM(k,L,c,p,d,s,0);

	context = new FHEcontext(m,p,r);
	buildModChain(*context, L, c);
	G = context->alMod.getFactorsOverZZ()[0];

	secretKey = new FHESecKey(*context);
	publicKey = secretKey;

	secretKey->GenSecKey(w);
	addSome1DMatrices(*secretKey); // compute key-switching matrices that we need

}


// /* =========   Tree Structure  ========= */

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

// /* =========	Homomorphic Algorithms for NOT, AND, OR, XOR, XNOR, and Compare ========= */

// NOT a = 1 - a
void NOT(Ctxt res, Ctxt op1, Ctxt encOne){
	res = encOne;
	// res.addCtxt(op1, true); // res = res - op1
	res -= op1;
}

// a AND b = a*b
Ctxt AND(Ctxt op1, Ctxt op2){
	// res = op1;
	// res *= op2;
	return op1 *= op2;
}

// a OR b= a+b-a*b
Ctxt OR(Ctxt res, Ctxt op1, Ctxt op2){
	res = AND(op1, op2);    // res = op1 * op2
	op1 += op2;             // op1 = op1 + op2
	op1.addCtxt(res, true); // op1 = op1 + op2 - op1 * op2
	return op1;
}

// a XOR b= a+b-2*a*b
Ctxt XOR(Ctxt res, Ctxt op1, Ctxt op2,  Ctxt encTwo){
	res = op1;
	res += op2;                     // res = op1 + op2
	op1.multiplyBy2(encTwo, op2);   // op1 = op1 * 2 * op2
	res.addCtxt(op1, true);
	return res;
}

// a XNOR b= a+(NOT b)-2*a*(NOT b)
Ctxt XNOR(Ctxt res, Ctxt op1, Ctxt op2, Ctxt encOne, Ctxt encTwo){
	encOne.addCtxt(op2, true);             // encOne = 1 - op2 (NOT op2)
	res = XOR(res, op1, encOne, encTwo);
	return res;
}

// /* =========	Homomorphic Algorithms for Compare and PathCost ========= */

Ctxt compare(Ctxt res, vector<Ctxt> x, vector<Ctxt> y, Ctxt encOne, Ctxt encTwo){

	// Create two vectors to store encrypted "bit-by-bit" algorithms: (<) and (=)

	int vec_len = x.size();     // TODO make sure x and y have the same size
	vector<Ctxt> less;
	vector<Ctxt> eq;

	// less: (x_i < y_i) = NOT x_i AND y_i
	//  eq:  (x_i = y_i) = x_i XNOR y_i

	for (int i = 0; i<vec_len; i++){
		//NOT(res, x[i], encOne);     // NOT x_i
		//res = AND(res, y[i]);       // res = NOT x_i AND y_i
		less.push_back(encOne);
		res = less[i];
		res.addCtxt(x[i], true);
		res *= y[i];
		less[i] = res;
		if (i>0){
			res = XNOR(res, x[i], y[i], encOne, encTwo);
			eq.push_back(res);
		}
	}

	cout << endl;

	// Execute comparison circuit:

	vector<Ctxt> comp;

	// Computing the inner AND operations

	comp.push_back(less[vec_len-1]);

	res = AND(eq[vec_len-2], less[vec_len-2]);
	comp.push_back(res);

	for (int i=vec_len-3; i>-1; i--)
	{
		res = eq[vec_len-2];        // x_l = y_l
		for (int j = vec_len-2; j>i; j--)
		{
			res = AND(res, eq[j-1]);
		}
		res = AND(res, less[i]);
		comp.push_back(res);
	}

	// Computing the outer OR operations

	res = comp[0];
	for (int i=1; i<vec_len; i++)
	{
		res = OR(res, res, comp[i]);
	}


	return res;
}

vector<node> pathCost(vector<node> T, Ctxt encZero, Ctxt encOne){
	Ctxt temp(*publicKey);
	Ctxt tValue(*publicKey);
	int nodeNum = T.size();
    timer.start();
	T[0].set_pathCost(encZero);

	for(int i=1; i<nodeNum; i++){
		if (	T[i].get_isRight()){		// if it is a right child, compute pc_node = pc_parent + (1-b_parent)
			temp = T[i].get_parent()->get_pathCost();
			encOne.addCtxt(T[i].get_parent()->get_vctxt(), true);	// encOne = 1 - b_parent
			temp+=encOne;
		} else {
			temp = T[i].get_parent()->get_pathCost();
			encOne=T[i].get_parent()->get_vctxt();	// encOne = b_parent
			temp+=encOne;
		}
		T[i].set_pathCost(temp);
	}
    timer.stop();

	ZZX result;
	for (int i=0; i<T.size(); i++){
		tValue = T[i].get_pathCost();		//get_vctxt();		//
		secretKey->Decrypt(result, tValue);
		cout << result[0] << "	";
	}
	return T;
}

ZZX OT(int leafNum, vector<node> & T, vector<Ctxt> ind, vector<Ctxt> blind){
	// computing the oblivious transfer to extract the result of tree evaluation.
	// each element in ind is a ciphertext of blinded pathCost (i.e. pc * random value).
	// ind is already populated with encryptions of random nums
	// each element in blind is a ciphertext of blinded classification value (i.e classVal + ind)

	Ctxt temp(*publicKey);
	int nodeNum = T.size();
	int n = nodeNum - leafNum;
	ZZX result;

	timer.start();
	for (int i = n; i<nodeNum; i++){
		if (T[i].get_isLeaf()){		// double check that we are computing on leafNodes only
			// computing the indicator: ind = pc * random
			temp = T[i].get_pathCost();
			ind[i-n] *=temp;
			// computing the blinded value: blind = classValue + ind
			temp = T[i].get_vctxt();
			temp += ind[i-n];
			blind.push_back(temp);
		}
	}
	timer.stop();

	cout << endl;
	cout << endl;
	cout << "Oblivious Transfer time: " << timer.elapsed_time() << " seconds." << endl;

	timer.start();
	for (int i = 0; i<ind.size(); i++){
		secretKey->Decrypt(result, ind[i]);
		if(result == to_ZZX(0))
			secretKey->Decrypt(result, blind[i]);
	}
	timer.stop();
	cout << endl;
	cout << "Result Retrieval and Decryption time: " << timer.elapsed_time() << " seconds." << endl;

	return result;

	//cout << endl << blind.size() << endl;
}

// /* =========	Main file: Demo	========= */

int main() // TODO: include variables to enable experimenting with different crypto-system parameters
{
	cout << endl;
	cout << "Program Started!!" << endl;
	cout << endl;

	char* bin;

	// ######## Start of FHE setup ########

	long p = 13;     // plaintext module
	long r = 1;
	long L = 16;    // number of levels
	long c = 2;
	long w = 64;
	long d = 0;
	long k = 128;
	long s = 0;

	timer.start();

	setupHElib( p,  r,  L,  c,  w,  d,  k,  s);

	timer.stop();
	cout << "KeyGen time: " << timer.elapsed_time() << " seconds." << endl;

	// ######## Creation of a decision tree example ########

	int height = 2;
	string type = "full";

	timer.start();

	vector<node> T = StructTree(height, type);

	timer.stop();
	cout << "TreeCreation time: " << timer.elapsed_time() << " seconds." << endl;

	// for now, use hard-coded info to determine the number of leafNode
	int leafNum = T[T.size()-1].get_value();
	int decisionNum = T.size()-leafNum;

	// ######## Pre-HE computations and setup ########

	// Create encrypted values of 1 and 2:
	Ctxt encZero(*publicKey);
	Ctxt encOne(*publicKey);
	Ctxt encTwo(*publicKey);

	publicKey->Encrypt(encZero, to_ZZX(0));
	publicKey->Encrypt(encOne, to_ZZX(1));

	for (int i=T.size()-leafNum; i<T.size();i++){
		T[i].set_vctxt(encZero);
	}

	bin = toBinary(2);

	ZZX TWO = createPolyFromCoeffsArray(bin);
	publicKey->Encrypt(encTwo, TWO);

	// Setup client (user) and server inputs

	//ZZX userValue, serverValue;
	cout << endl;
	cout << endl;
	cout << "Let's Compare Two Encrypted Values" << endl;
//	cout << "Please Enter Your Value: " << endl;
//	cin >> unum;
	cout << endl;

	uniform_int_distribution<long> diss(8, 15);  // set the range of random values

	vector<int> serverValue;
	for (int i=0; i<decisionNum; i++){
		serverValue.push_back(diss(gen));	// for now, let all values be > 8
		cout << serverValue[i] << " ";
	}
	cout << endl << endl;


	// Transform number into binary string and set ZZX coeffs from the binary string

	ZZX temp, isLess;
	int len;
	int unum, snum;
	//snum = 11;          // set a server value to compare against

	for (int i=0; i<decisionNum; i++){

		vector<ZZX> sBits;
		vector<ZZX> uBits;

		cout << "DecisionNode #" << i << endl;
		snum = serverValue[i];
		unum = T[i].get_value();

		bin = toBinary(snum);
		cout << endl;
		//printBits(bin);
		//cout << endl;

		// Determine the length of the binary string for bit-by-bit encryption
		len = strlen(bin);
		//cout << endl << len << endl;

		for (int i = 0; i<strlen(bin)-1; i++){
			SetCoeff(temp, 0, to_ZZ(bin[i]-'0'));
			sBits.push_back(temp);
			//cout << sBits[i] << " ";
		}
		//cout << endl;
		//cout << endl;

		bin = toBinary(unum, len-1);
		//cout << endl << strlen(bin) << endl;
		//printBits(bin);
		//cout << endl;

		for (int i = 0; i<strlen(bin)-1; i++){
			SetCoeff(temp, 0, to_ZZ(bin[i]-'0'));
			uBits.push_back(temp);
			//cout << uBits[i] << " ";
		}
		//cout << endl;

		//	userValue = createPolyFromCoeffsArray(bin);
		//	printZZX(userValue);
		//cout << endl;

		// ######## Homomorphically encrypting individual bits in binary strings ########

		Ctxt Value(*publicKey);      // Stores temp encrypted values
		Ctxt encLess(*publicKey);    // Store the encryptied 1-bit comp result

		vector<Ctxt> uValue;
		vector<Ctxt> sValue;

		//timer->start();
		timer.start();
		for (int i=0; i<len-1; i++){
			//SetCoeff(temp, 0, userValue[i]);
			publicKey->Encrypt(Value, uBits[i]);//temp);
			uValue.push_back(Value);

			//SetCoeff(temp, 0, serverValue[i]);
			publicKey->Encrypt(Value, sBits[i]);
			sValue.push_back(Value);
		}
		timer.stop();
		cout << endl;
		cout <<"Bit-by-bit Encryption time: " << timer.elapsed_time() << " seconds." << endl;
		//timer->stop("Encryption", false);

		// ######## Performing homomorphic comparison on encrypted values ########

		//timer->start();
		timer.start();
		encLess = compare(encLess, uValue, sValue, encOne, encTwo);
		// timer->stop("Secure Comparison", false);

		timer.stop();
		cout << endl;
		cout << "Secure Comparison time: " << timer.elapsed_time() << " seconds." << endl;

		T[i].set_vctxt(encLess);

		// Testing correctness of comparison

		//timer->start();
		timer.start();
		secretKey->Decrypt(isLess, encLess);
		//timer->stop("Decryption", false);

		timer.stop();
		cout << endl;
		cout << "Result Decryption time: " << timer.elapsed_time() << " seconds." << endl;

		// Verification step: TODO: fix when numbers are equal, it yields wrong result!!
		cout << endl;
		cout << endl;
		cout << "Is " << unum << " LESS THAN " << snum << " ?" << endl;
		cout << "The result is " << isLess[0] << endl << endl;

	}	// end of the for loop for comparing each decision node!!


	// ######## Performing Path Cost and Computing OT on encrypted values ########

	// encrypt classification values/labels

    Ctxt tValue(*publicKey);
    Ctxt encTemp(*publicKey);

	for (int i=leafNum; i<T.size(); i++){
			publicKey->Encrypt(tValue, to_ZZX(T[i].get_value()));
			T[i].set_vctxt(tValue);
		}


	// Path Cost

	ZZX result;
//
//		T = pathCost(T, encZero, encOne);
//		cout << endl;
//		cout << "Path Cost time: " << timer.elapsed_time() << " seconds." << endl;

	timer.start();
	T[0].set_pathCost(encZero);
	publicKey->Encrypt(encOne, to_ZZX(1));

	for(int i=1; i<T.size(); i++){
		if (	T[i].get_isRight()){		// if it is a right child, compute pc_node = pc_parent + (1-b_parent)
			encTemp = T[i].get_parent()->get_pathCost();
			tValue = encOne;
			tValue.addCtxt(T[i].get_parent()->get_vctxt(), true);	// encOne = 1 - b_parent
			encTemp+=tValue;
		} else {
			encTemp = T[i].get_parent()->get_pathCost();
			tValue=T[i].get_parent()->get_vctxt();	// encOne = b_parent
			encTemp+=tValue;
		}
		T[i].set_pathCost(encTemp);
	}
	timer.stop();

	cout << "Path cost: " << endl;
	for (int i=0; i<T.size(); i++){
		if (T[i].get_isLeaf() ){
		tValue = T[i].get_pathCost();		//get_vctxt();		//
		secretKey->Decrypt(result, tValue);
		cout << result[0] << "	";
		}
	}

//	 // OT and Evaluation Result Retrieval:
//
//	vector<Ctxt> ind;
//	vector<Ctxt> blind;
//	int tempint;
//
//
//	uniform_int_distribution<long> disss(1, 10);
//	cout << endl << "Random values for leaf nodes: " << endl ;
//
//	for (int i = 0; i<leafNum; i++){
//		tempint = disss(gen);
//		cout << tempint << " ";
//		publicKey->Encrypt(tValue, to_ZZX(tempint));
//		ind.push_back(tValue);
//	}
//
//	ZZX result;
//	result =  OT(leafNum, T, ind, blind);
//	cout << endl;
//	cout << "Oblivious Transfer time: " << timer.elapsed_time() << " seconds." << endl;
//
//	cout << endl << endl << "The Class Label is:  " << endl << endl;
//	printZZX(result);


	cout << endl << endl;
	cout << "Program Ended Successfully!!" << endl;
	cout << endl;

	return 0;


}

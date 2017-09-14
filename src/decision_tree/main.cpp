#include<iostream>
#include<FHE.h>
#include<vector>
#include <string>
#include <math.h>
#include <random>
#include <cstddef>
#include <sys/time.h>

#include "decision_tree/tree.h"
#include "decision_tree/util.h"
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

void pathCost(vector<node> & T, Ctxt encZero, Ctxt encOne){
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
}

//ZZX OT(int leafNum, vector<node> T, vector<Ctxt> ind, vector<Ctxt> blind){
//	// computing the oblivious transfer to extract the result of tree evaluation.
//	// each element in ind is a ciphertext of blinded pathCost (i.e. pc * random value).
//	// ind is already populated with encryptions of random nums
//	// each element in blind is a ciphertext of blinded classification value (i.e classVal + ind)
//
//	Ctxt temp(*publicKey);
//	int nodeNum = T.size();
//	int n = nodeNum - leafNum;
//	ZZX result;
//
//	timer.start();
//	for (int i = n; i<nodeNum; i++){
//		if (T[i].get_isLeaf()){		// double check that we are computing on leafNodes only
//			// computing the indicator: ind = pc * random
//			temp = T[i].get_pathCost();
//			ind[i-n] *=temp;
//			// computing the blinded value: blind = classValue + ind
//			temp = T[i].get_vctxt();
//			temp += ind[i-n];
//			blind.push_back(temp);
//		}
//	}
//	timer.stop();
//
//	cout << endl;
//	cout << endl;
//	cout << "Oblivious Transfer time: " << timer.elapsed_time() << " seconds." << endl;
//
//	timer.start();
//	for (int i = 0; i<ind.size(); i++){
//		secretKey->Decrypt(result, ind[i]);
//		if(result == to_ZZX(0))
//			secretKey->Decrypt(result, blind[i]);
//	}
//	timer.stop();
//	cout << endl;
//	cout << "Result Retrieval and Decryption time: " << timer.elapsed_time() << " seconds." << endl;
//
//	return result;
//
//	//cout << endl << blind.size() << endl;
//}

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

	vector<node> T = StructTree(height, type, *publicKey);

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
		printBits(bin);
		cout << endl;

		// Determine the length of the binary string for bit-by-bit encryption
		len = strlen(bin);
		//cout << endl << len << endl;

		for (int i = 0; i<strlen(bin)-1; i++){
			SetCoeff(temp, 0, to_ZZ(bin[i]-'0'));
			sBits.push_back(temp);
			cout << sBits[i] << " ";
		}
		cout << endl;

		bin = toBinary(unum, len-1);
		//cout << endl << strlen(bin) << endl;
		printBits(bin);
		cout << endl;

		for (int i = 0; i<strlen(bin)-1; i++){
			SetCoeff(temp, 0, to_ZZ(bin[i]-'0'));
			uBits.push_back(temp);
			cout << uBits[i] << " ";
		}
		cout << endl;

		//	userValue = createPolyFromCoeffsArray(bin);
		//	printZZX(userValue);
		cout << endl;

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

		pathCost(&T, encZero, encOne);
		cout << endl;
		cout << "Path Cost time: " << timer.elapsed_time() << " seconds." << endl;


//	timer.start();
//	T[0].set_pathCost(encZero);
//
//	for(int i=1; i<T.size(); i++){
//		if (	T[i].get_isRight()){		// if it is a right child, compute pc_node = pc_parent + (1-b_parent)
//			encTemp = T[i].get_parent()->get_pathCost();
//			encOne.addCtxt(T[i].get_parent()->get_vctxt(), true);	// encOne = 1 - b_parent
//			encTemp+=encOne;
//		} else {
//			encTemp = T[i].get_parent()->get_pathCost();
//			encOne=T[i].get_parent()->get_vctxt();	// encOne = b_parent
//			encTemp+=encOne;
//		}
//		T[i].set_pathCost(encTemp);
//	}
//	timer.stop();
//
//	ZZX result;
//	for (int i=0; i<T.size(); i++){
//		tValue = T[i].get_pathCost();		//get_vctxt();		//
//		secretKey->Decrypt(result, tValue);
//		cout << result[0] << "	";
//	}
//

//	// OT and Evaluation Result Retrieval:
//
//	vector<Ctxt> ind;
//	vector<Ctxt> blind;
//	int tempint;
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
//	//	cout << endl << ind.size() << endl;
//	//	cout << endl << blind.size() << endl;
//
//	//Ctxt encTemp(*publicKey);
//	int n = T.size() - leafNum;
//	//ZZX result;
//
//	timer.start();
//	for (int i = n; i<T.size(); i++){
//		if (T[i].get_isLeaf()){		// double check that we are computing on leafNodes only
//			// computing the indicator: ind = pc * random
//			encTemp = T[i].get_pathCost();
//			ind[i-n] *=encTemp;
//			// computing the blinded value: blind = classValue + ind
//			encTemp = T[i].get_vctxt();
//			encTemp += ind[i-n];
//			blind.push_back(encTemp);
//		}
//	}
//	timer.stop();
//
//	cout << endl;
//	cout << endl;
//	cout << "Oblivious Transfer time: " << timer.elapsed_time() << " seconds." << endl;
//
//	timer.start();
//	for (int i = 0; i<ind.size(); i++){
//		secretKey->Decrypt(result, ind[i]);
//		if(result == to_ZZX(0))
//			secretKey->Decrypt(result, blind[i]);
//	}
//	timer.stop();
//	cout << endl;
//	cout << "Result Retrieval and Decryption time: " << timer.elapsed_time() << " seconds." << endl;
//
//
////	result =  OT(leafNum, T, ind, blind);
////	cout << endl;
////	cout << "Oblivious Transfer time: " << timer.elapsed_time() << " seconds." << endl;
//
//	cout << endl << endl << "The Class Label is:  " << endl << endl;
//	printZZX(result);
//
//
//	cout << endl << endl << endl;
//

	cout << "Program Ended Successfully!!" << endl;
	cout << endl;

	return 0;


}


//	for (int i=0; i<T.size(); i++){
//		tValue = T[i].get_pathCost();		//get_vctxt();		//
//		secretKey->Decrypt(result, tValue);
//		cout << result[0] << "	";
//	}


//
//	for (int i = 0; i<ind.size(); i++){
//		secretKey->Decrypt(result, ind[i]);
//		if(result == to_ZZX(0))
//			secretKey->Decrypt(result, blind[i]);
//	}
//
//	cout << endl << endl << "The Class Label is:  " << result[0] << endl << endl;
//

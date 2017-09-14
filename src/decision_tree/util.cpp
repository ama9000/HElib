
#include <decision_tree/util.h>
#include <vector>
#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

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



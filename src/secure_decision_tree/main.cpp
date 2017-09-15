//
// Created by ph on 9/14/17.
//

#include "Crypto.h"
#include "SecComparison.h"
#include "util.h"
#include "Tree.h"
#include "timing.h"

#include <iostream>


using namespace std;


void testBinary() {

/*    int test = 200;
    while(test>0) {
        unsigned int test2 = Helper::getRandomInt(0, 100);
        cout << test2 << endl;
        char* bin = Helper::intToBinary(test2);
        cout << bin << endl;
        test--;
    }
*/
}

void testHE(Crypto* cryptoObj, timing* timer){

    FHEPubKey* pubKey = cryptoObj->get_pubKey();
    SecComparison secComparison;

//    Ctxt a(*(cryptoObj->get_pubKey()));
//    Ctxt b(*(cryptoObj->get_pubKey()));
//    Ctxt encOR(*(cryptoObj->get_pubKey()));

    Ctxt a(*pubKey);
    Ctxt b(*pubKey);
    Ctxt encLogic(*pubKey);
    ZZX result;

    timer->start();
    cryptoObj->encrypt(a, to_ZZX(1));
    timer->stop("Encrypt", false);

    cout << to_ZZX(1)<< endl;

    timer->start();
    cryptoObj->decrypt(result, a);
    timer->stop("Decrypt", false);

    cout << result[0] << endl;

    cryptoObj->get_pubKey()->Encrypt(a, to_ZZX(0));
    cryptoObj->get_pubKey()->Encrypt(b, to_ZZX(1));

    timer->start();
    encLogic = secComparison.NOT(a);
    timer->stop("NOT", false);

    cryptoObj->get_secKey()->Decrypt(result, encLogic);

    cout << result[0] << endl;

    timer->start();
    encLogic = secComparison.OR(encLogic, a,b);
    timer->stop("OR", false);

    cryptoObj->get_secKey()->Decrypt(result, encLogic);

    cout << result[0] << endl;

    timer->start();
    encLogic = secComparison.AND(a, b);
    timer->stop("AND", false);

    cryptoObj->get_secKey()->Decrypt(result, encLogic);

    cout << result[0] << endl;

    timer->start();
    encLogic = secComparison.XOR(encLogic, a, b);
    timer->stop("XOR", false);

    cryptoObj->get_secKey()->Decrypt(result, encLogic);

    cout << result[0] << endl;

    timer->start();
    encLogic = secComparison.XNOR(encLogic, a, b);
    timer->stop("XNOR", false);

    cryptoObj->get_secKey()->Decrypt(result, encLogic);

    cout << result[0] << endl;

}

void testCompare(Crypto* cryptoObj, timing* timer){

    SecComparison secComparison;
    Ctxt a(*(cryptoObj->get_pubKey()));
    Ctxt compareVal(*(cryptoObj->get_pubKey()));
    vector<Ctxt> ct1;
    vector<Ctxt> ct2;
    ZZX result;


    unsigned int test1 =  Helper::getRandomInt(5, 15);
    cout << test1 << endl;
    char* bin = Helper::intToBinary(test1,4);
    cout << bin << endl;

    cout << "Encrypting and Decrypting test_value_1" << endl;


    for(int i=0; i<strlen(bin); i++){
        cryptoObj->get_pubKey()->Encrypt(a, to_ZZX(bin[i]-'0'));
        ct1.push_back(a);
        cryptoObj->decrypt(result, ct1[i]);
        cout << result[0] << "  ";
    }

    cout << endl;


    unsigned int test2 = Helper::getRandomInt(5, 15);
    cout << test2 << endl;
    bin = Helper::intToBinary(test2, 4);
    cout << bin << endl;

    cout << "Encrypting and Decrypting test_value_2" << endl;

    for(int i=0; i<strlen(bin); i++){
        cryptoObj->encrypt(a, to_ZZX(bin[i]-'0'));
        ct2.push_back(a);
        cryptoObj->decrypt(result, ct2[i]);
        cout << result[0] << "  ";
    }

    cout << endl;

    cout << "ct1 length: " << ct1.size() << endl;
    cout << "ct2 length: " << ct2.size() << endl;

    a = secComparison.compare(compareVal, ct1, ct2, cryptoObj);
    cryptoObj->decrypt(result,a);

    cout << endl << endl << result[0] << endl;
}

void testTree(timing* timer) {
    timer->start();
    Tree* tree = new Tree(COMPLETE, 2);
    tree->print_tree();
    timer->stop("tree building", false);
}

int main() {
    cout << endl;
    cout << "Program Started!!" << endl;
    cout << endl;

    HEparams params;
    Crypto* cryptoObj = new Crypto(&params);


//
      timing timer;
//
//    testTree(&timer);
//
//    timer.show();

    //testHE(cryptoObj, &timer);

    //TODO test individual HE addition, subtraction, multiplication, constant multiplication
    //timer.show();
    testCompare(cryptoObj, &timer);















}

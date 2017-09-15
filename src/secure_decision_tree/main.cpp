//
// Created by ph on 9/14/17.
//

#include "crypto/Crypto.h"
#include "crypto/SecComparison.h"
#include "common/util.h"
#include "tree/Tree.h"
#include "common/timing.h"

#include <iostream>


using namespace std;





void testBinary() {
    int test = 200;
    while(test>0) {
        unsigned int test2 = Helper::getRandomInt(0, 100);
        cout << test2 << endl;
        char* bin = Helper::intToBinary(test2);
        cout << bin << endl;
        test--;
    }
}

void testHE(timing* timer){
    HEparams params;
    Crypto* cryptoObj = new Crypto(&params);

    Ctxt a(*(cryptoObj->get_pubKey()));

    timer->start();
    cryptoObj->encrypt(a, to_ZZX(1));
    timer->stop("Encrypt", false);

    cout << to_ZZX(1)<< endl;

    ZZX result;
    timer->start();
    cryptoObj->decrypt(result, a);
    timer->stop("Decrypt", false);

    cout << result[0] << endl;

}


void testTree(timing* timer) {
    timer->start();
    Tree* tree = new Tree(COMPLETE, 5);
    tree->print_tree();
    timer->stop("TreeBuilding", false);
}





int main() {
    cout << endl;
    cout << "Program Started!!" << endl;
    cout << endl;

    timing* timer = new timing();

    testTree(timer);

    //testHE(timer);




    timer->show();

















}
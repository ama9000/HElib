//
// Created by ph on 9/14/17.
//

#include "Crypto.h"
#include "SecComparison.h"
#include "util.h"
#include "Node.h"
#include <iostream>


using namespace std;




int main() {
    cout << endl;
    cout << "Program Started!!" << endl;
    cout << endl;

/*    int test = 200;
    while(test>0) {
        unsigned int test2 = Helper::getRandomInt(0, 100);
        cout << test2 << endl;
        char* bin = Helper::intToBinary(test2);
        cout << bin << endl;
        test--;
    }
*/

    HEparams params;
    Crypto* cryptoObj = new Crypto(&params);
    //SecComparison secComparison;

    Ctxt a(*(cryptoObj->get_pubKey()));

//    Ctxt b(cryptoObj.get_pubKey());
//    Ctxt h(cryptoObj.get_pubKey());

   // Ctxt *a, *b, *h;
//
    //cryptoObj->encrypt(a, to_ZZX(1));

    cryptoObj->get_pubKey()->Encrypt(a, to_ZZX(1));

    cout << to_ZZX(1)<< endl;

    ZZX result;

    //cryptoObj->decrypt(result, a);

    cryptoObj->get_secKey()->Decrypt(result, a);

    cout << result[0] << endl;


    //cryptoObj.encrypt(b, to_ZZX(1));

//
//    //h = secComparison.OR(h, a, b);
//
//    //ZZX result =  to_ZZX(1);
//
    //ZZX result = cryptoObj.HEdecrypt(b);

    //cout << endl << result << endl;





    //tree tree







}
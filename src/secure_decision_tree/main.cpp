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

    //cout << Helper::getRandomInt(0,15) << endl;

    HEparams params;
    Crypto* cryptoObj = new Crypto(&params);
    SecComparison secComparison;

    Ctxt a(*(cryptoObj->get_pubKey()));
    Ctxt b(*(cryptoObj->get_pubKey()));
    Ctxt encOR(*(cryptoObj->get_pubKey()));

    cryptoObj->get_pubKey()->Encrypt(a, to_ZZX(0));
    cryptoObj->get_pubKey()->Encrypt(b, to_ZZX(1));

    ZZX result;

    encOR = secComparison.NOT(a);

    cryptoObj->get_secKey()->Decrypt(result, encOR);

    cout << result[0] << endl;






}


//cryptoObj->decrypt(result, a);
//    Ctxt b(cryptoObj.get_pubKey());
//    Ctxt h(cryptoObj.get_pubKey());

// Ctxt *a, *b, *h;
//
//cryptoObj->encrypt(a, to_ZZX(1));
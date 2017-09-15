//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_CRYPTO_H
#define SECUREDECISIONTREE_CRYPTO_H

#include <FHE.h>
#include <NTL/ZZX.h>

struct HEparams{
    long p = 2;          // defining the plaintext space modular
    long r = 1;
    long L = 16;              // number of levels (i.e. number of prime modular in modChain)
    long c = 2;
    long w = 64;
    long d = 0;
    long k = 128;
    long s = 0;
};



////long p = 13;     // plaintext module
//long r = 1;
////long L = 16;    // number of levels
//long c = 2;
//long w = 64;
//long d = 0;
//long k = 128;
//long s = 0;

class Crypto{

private:
     HEparams* params;
     FHEcontext* context;
     FHESecKey* secretKey;
     FHEPubKey* publicKey;
     ZZX G;

public:
    Crypto(HEparams *params);

    // accessing the keys
    FHEPubKey* get_pubKey(){return this->publicKey;}

    FHESecKey* get_secKey(){return this->secretKey;}

    // encryption
     void encrypt(Ctxt cipher_text, ZZX plain_text);

    // decryption
     void decrypt(ZZX plain_text, Ctxt cipher_text);
};

#endif //SECUREDECISIONTREE_CRYPTO_H

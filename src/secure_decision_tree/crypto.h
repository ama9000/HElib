//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_CRYPTO_H
#define SECUREDECISIONTREE_CRYPTO_H

struct HEparams{
    long p = 2;          // defining the plaintext space modular
    long r = 1;
    long L = 16;              // number of levels (i.e. number of prime modular in modChain)
    long c = 0;
    long w = 64;
    long d = 0;
    long k = 128;
    long s = 0;
};

class Crypto{

private:
    const FHEcontext* context;
    const FHESecKey* secretKey;
    const FHEPubKey* publicKey;
    const ZZX G;

public:
    const void setupHElib( HEparams *params);

    // encryption
    const Ctxt HEencrypt(ZZX plain_text ,FHEPubKey* publicKey);

    // decryption
    const ZZX HEdecrypt(Ctxt cipher_text ,FHESecKey* secretKey);

};

#endif //SECUREDECISIONTREE_CRYPTO_H

//
// Created by ph on 9/14/17.
//

#include "Crypto.h"
#include <FHE.h>


using namespace std;
using namespace NTL;

//long m = FindM(k,L,c,p,d,s,0);
//////long p = 13;     // plaintext module
//long r = 1;
////long L = 16;    // number of levels
//long c = 2;
//long w = 64;
//long d = 0;
//long k = 128;
//long s = 0;
//context = new FHEcontext(m,p,r);
//buildModChain(*context, L, c);
//G = context->alMod.getFactorsOverZZ()[0];
//
//secretKey = new FHESecKey(*context);
//publicKey = secretKey;
//
//secretKey->GenSecKey(w);
//addSome1DMatrices(*secretKey); // compute key-switching matrices that we need


Crypto::Crypto(HEparams* params){
    long m = FindM(params->k,params->L,params->c,params->p,params->d,params->s,0);

    this->context = new FHEcontext(m,params->p,params->r);
    buildModChain(*(this->context), params->L, params->c);
    this->G = context->alMod.getFactorsOverZZ()[0];

    this->secretKey = new FHESecKey(*(this->context));
    this->publicKey = this->secretKey;

    this->secretKey->GenSecKey(params->w);
    addSome1DMatrices(*(this->secretKey)); // compute key-switching matrices that we need
}

void Crypto::encrypt(Ctxt cipher_text, const ZZX plain_text){
    this->get_pubKey()->Encrypt(cipher_text, plain_text);
}

// decryption
void Crypto::decrypt(ZZX plain_text, const Ctxt cipher_text){
    this->get_secKey()->Decrypt(plain_text, cipher_text);
}

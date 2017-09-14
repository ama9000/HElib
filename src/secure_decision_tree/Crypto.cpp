//
// Created by ph on 9/14/17.
//

#include "Crypto.h"
#include <FHE.h>


using namespace std;
using namespace NTL;


const void Crypto::setupHElib( HEparams *params){

    long m = FindM(params.p,params.L,params.c,params.k,params.d,params.s,0);

    this.context = new FHEcontext(m,params.p,params.r);
    buildModChain(*context, params.L, params.c);
    this.G = context->alMod.getFactorsOverZZ()[0];

    this.secretKey = new FHESecKey(*context);
    this.publicKey = this.secretKey;

    this.secretKey->GenSecKey(params.w);
    addSome1DMatrices(this->secretKey); // compute key-switching matrices that we need

}

const Ctxt Crypto::HEencrypt(ZZX plain_text ,FHEPubKey* publicKey){

}

// decryption
const ZZX Crypto::HEdecrypt(Ctxt cipher_text ,FHESecKey* secretKey){

}

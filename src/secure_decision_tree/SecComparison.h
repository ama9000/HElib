//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_SECCOMPARISON_H
#define SECUREDECISIONTREE_SECCOMPARISON_H

#include <FHE.h>
#include "Crypto.h"

class SecComparison{

public:

    Ctxt NOT(Ctxt ctxt_1);

    Ctxt AND(Ctxt ctxt_1, Ctxt ctxt_2);

    Ctxt OR(Ctxt ctxt_OR, Ctxt ctxt_1, Ctxt ctxt_2);

    Ctxt XOR(Ctxt ctxt_XOR, Ctxt ctxt_1, Ctxt ctxt_2);

    Ctxt XNOR(Ctxt ctxt_XNOR, Ctxt ctxt_1, Ctxt ctxt_2);

    Ctxt compare(Ctxt ctxt_compare, vector<Ctxt> ctxt_input, vector<Ctxt> ctxt_threshold, Crypto* cryptobj);

};


//class PathCost{
//
//public:
//
//    const Ctxt pathCost(vector<node> T, Ctxt encZero, Ctxt encOne);
//
//
//    };


#endif //SECUREDECISIONTREE_SECCOMPARISON_H

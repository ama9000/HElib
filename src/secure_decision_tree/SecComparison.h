//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_SECCOMPARISON_H
#define SECUREDECISIONTREE_SECCOMPARISON_H=

#include <FHE.h>

class SecComparison{

public:

     Ctxt NOT(Ctxt op1);

     Ctxt AND(Ctxt op1, Ctxt op2);

     Ctxt OR(Ctxt res, Ctxt op1, Ctxt op2);

     Ctxt XOR(Ctxt res, Ctxt op1, Ctxt op2);

     Ctxt XNOR(Ctxt res, Ctxt op1, Ctxt op2);

    // Ctxt* compare(Ctxt* res, vector<Ctxt*> x, vector<Ctxt*> y);

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

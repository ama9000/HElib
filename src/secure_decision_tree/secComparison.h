//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_SECCOMPARISON_H
#define SECUREDECISIONTREE_SECCOMPARISON_H=

class HElogic{

public:

    const Ctxt NOT(Ctxt op1);

    const Ctxt AND(Ctxt op1, Ctxt op2);

    const Ctxt OR(Ctxt res, Ctxt op1, Ctxt op2);

    const Ctxt XOR(Ctxt res, Ctxt op1, Ctxt op2,  Ctxt encTwo);

    const Ctxt XNOR(Ctxt res, Ctxt op1, Ctxt op2, Ctxt encOne, Ctxt encTwo);

};


//class HEalg{
//
//public:
//    const Ctxt compare(Ctxt res, vector<Ctxt> x, vector<Ctxt> y, Ctxt encOne, Ctxt encTwo);
//
//    const Ctxt pathCost(vector<node> T, Ctxt encZero, Ctxt encOne){);
//
//};


#endif //SECUREDECISIONTREE_SECCOMPARISON_H

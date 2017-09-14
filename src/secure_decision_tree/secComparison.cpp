//
// Created by ph on 9/14/17.
//
#include "secComparison.h"
#include<FHE.h>

using namespace std;
using namespace NTL;

// NOT a = 1 - a
const Ctxt HElogic::NOT(Ctxt *op1){
    op1.negate();
    op1.addConstant(to_ZZ(1));
    return op1;
    // res.addCtxt(op1, true); // res = res - op1
    //res -= op1;
}

// a AND b = a*b
const Ctxt HElogic::AND(Ctxt op1, Ctxt op2)){
    // res = op1;
    // res *= op2;
    return op1 *= op2;
}

// a OR b= a+b-a*b
const Ctxt HElogic::OR(Ctxt res, Ctxt op1, Ctxt op2){
    res = AND(op1, op2);    // res = op1 * op2
    op1 += op2;             // op1 = op1 + op2
    op1.addCtxt(res, true); // op1 = op1 + op2 - op1 * op2
    return op1;
}

// a XOR b= a+b-2*a*b
const Ctxt XOR(Ctxt res, Ctxt op1, Ctxt op2,  Ctxt encTwo){
    res = op1;
    res += op2;                     // res = op1 + op2
    op1.multiplyBy2(encTwo, op2);   // op1 = op1 * 2 * op2
    res.addCtxt(op1, true);
    return res;
}

// a XNOR b= a+(NOT b)-2*a*(NOT b)
const Ctxt XNOR(Ctxt res, Ctxt op1, Ctxt op2, Ctxt encOne, Ctxt encTwo){
    op2 = NOT(op2);
    res = XOR(res, op1, encOne, encTwo);
    return res;
}

//
//Ctxt compare(Ctxt res, vector<Ctxt> x, vector<Ctxt> y, Ctxt encOne, Ctxt encTwo){
//
//    // Create two vectors to store encrypted "bit-by-bit" algorithms: (<) and (=)
//
//    int vec_len = x.size();     // TODO make sure x and y have the same size
//    vector<Ctxt> less;
//    vector<Ctxt> eq;
//
//    // less: (x_i < y_i) = NOT x_i AND y_i
//    //  eq:  (x_i = y_i) = x_i XNOR y_i
//
//    for (int i = 0; i<vec_len; i++){
//        //NOT(res, x[i], encOne);     // NOT x_i
//        //res = AND(res, y[i]);       // res = NOT x_i AND y_i
//        less.push_back(encOne);
//        res = less[i];
//        res.addCtxt(x[i], true);
//        res *= y[i];
//        less[i] = res;
//        if (i>0){
//            res = XNOR(res, x[i], y[i], encOne, encTwo);
//            eq.push_back(res);
//        }
//    }
//
//    cout << endl;
//
//    // Execute comparison circuit:
//
//    vector<Ctxt> comp;
//
//    // Computing the inner AND operations
//
//    comp.push_back(less[vec_len-1]);
//
//    res = AND(eq[vec_len-2], less[vec_len-2]);
//    comp.push_back(res);
//
//    for (int i=vec_len-3; i>-1; i--)
//    {
//        res = eq[vec_len-2];        // x_l = y_l
//        for (int j = vec_len-2; j>i; j--)
//        {
//            res = AND(res, eq[j-1]);
//        }
//        res = AND(res, less[i]);
//        comp.push_back(res);
//    }
//
//    // Computing the outer OR operations
//
//    res = comp[0];
//    for (int i=1; i<vec_len; i++)
//    {
//        res = OR(res, res, comp[i]);
//    }
//
//
//    return res;
//}

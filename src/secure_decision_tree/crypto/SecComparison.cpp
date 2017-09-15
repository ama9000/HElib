//
// Created by ph on 9/14/17.
//
#include "SecComparison.h"

using namespace std;
using namespace NTL;

// NOT a = 1 - a
 Ctxt* SecComparison::NOT(Ctxt* op1){
    op1->negate();
    op1->addConstant(to_ZZ(1));
    return op1;
    // res.addCtxt(op1, true); // res = res - op1
    //res -= op1;
}

// a AND b = a*b
 Ctxt* SecComparison::AND(Ctxt* op1, Ctxt* op2){
    // res = op1;
    // res *= op2;
    *op1 *= *op2;
    return op1;
}

// a OR b= a+b-a*b
 Ctxt* SecComparison::OR(Ctxt* res, Ctxt* op1, Ctxt* op2){
    res = AND(op1, op2);    // res = op1 * op2
    *op1 += *op2;             // op1 = op1 + op2
    op1->addCtxt(*res, true); // op1 = op1 + op2 - op1 * op2
    return op1;
}

// a XOR b= a+b-2*a*b
 Ctxt* SecComparison::XOR(Ctxt* res, Ctxt* op1, Ctxt* op2){
    res = op1;
    *res += *op2;                     // res = op1 + op2
    op1->multByConstant(to_ZZ(2));   // op1 = 2*op1
    //op1.multiplyBy(op2);          // op1 = 2* op1 * op2
    *op1 *= *op2;                     // op1 = 2* op1 * op2
    res->addCtxt(*op1, true);         // res = op1 + op2 - 2 * op1 * op2
    return res;
}

// a XNOR b= a+(NOT b)-2*a*(NOT b)
 Ctxt* SecComparison::XNOR(Ctxt* res, Ctxt* op1, Ctxt* op2){
    op2 = NOT(op2);
    res = XOR(res, op1, op2);
    return res;
}

////
// Ctxt* SecComparison::compare(Ctxt* res, vector<Ctxt*> x, vector<Ctxt*> y){
//
//    // Create two vectors to store encrypted "bit-by-bit" algorithms: (<) and (=)
//    // less: (x_i < y_i) = NOT x_i AND y_i
//    //  eq:  (x_i = y_i) = x_i XNOR y_i
//
//    int vec_len = x.size();
//    vector<Ctxt> less;
//    vector<Ctxt> eq;
//
//
//    for (int i = 0; i<vec_len; i++){
//        //NOT(res, x[i], encOne);     // NOT x_i
//        //res = AND(res, y[i]);       // res = NOT x_i AND y_i
//        res = NOT(x[i]);
//        res *= y[i];
//        less.push_back(res);
//        if (i>0){
//            res = XNOR(res, x[i], y[i]);
//            eq.push_back(res);
//        }
//    }
//
//    cout << endl;
//
//    // Execute comparison circuit:
//    vector<Ctxt> comp;
//
//    // Computing the inner AND operations
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

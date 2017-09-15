//
// Created by ph on 9/14/17.
//
#include "SecComparison.h"


using namespace std;
using namespace NTL;

// NOT a = 1 - a
 Ctxt SecComparison::NOT(Ctxt ctxt_1){
    ctxt_1.negate();
    ctxt_1.addConstant(to_ZZ(1));
    return ctxt_1;
}

// a AND b = a*b
 Ctxt SecComparison::AND(Ctxt ctxt_1, Ctxt ctxt_2){
    ctxt_1 *= ctxt_2;
    return ctxt_1;
}

// a OR b= a+b-a*b
 Ctxt SecComparison::OR(Ctxt ctxt_OR, Ctxt ctxt_1, Ctxt ctxt_2){
    ctxt_OR = this->AND(ctxt_1, ctxt_2);          // enc_OR = ctxt_1 * ctxt_2
    ctxt_1 += ctxt_2;                       // ctxt_1 = ctxt_1 + ctxt_2
    ctxt_1.addCtxt(ctxt_OR, true);          // ctxt_1 = ctxt_1 + ctxt_2 - op1 * ctxt_2
    return ctxt_1;
}

// a XOR b= a+b-2*a*b
 Ctxt SecComparison::XOR(Ctxt ctxt_XOR, Ctxt ctxt_1, Ctxt ctxt_2){
    ctxt_XOR = ctxt_1;
    ctxt_XOR += ctxt_2;                      // ctxt_XOR = ctxt_1 + ctxt_2
    ctxt_1.multByConstant(to_ZZ(2));         // ctxt_1 = 2*ctxt_1
    ctxt_1 *= ctxt_2;                        // ctxt_1 = 2* ctxt_1 * ctxt_2
    ctxt_XOR.addCtxt(ctxt_1, true);          // ctxt_XOR = ctxt_1 + ctxt_2 - 2 * ctxt_1 * ctxt_2
    return ctxt_XOR;
}

// a XNOR b= a+(NOT b)-2*a*(NOT b)
 Ctxt SecComparison::XNOR(Ctxt ctxt_XNOR, Ctxt ctxt_1, Ctxt ctxt_2){
    ctxt_2 = this->NOT(ctxt_2);
    ctxt_XNOR = this->XOR(ctxt_XNOR, ctxt_1, ctxt_2);
    return ctxt_XNOR;
}

//
 Ctxt SecComparison::compare(Ctxt ctxt_compare, vector<Ctxt> ctxt_input, vector<Ctxt> ctxt_threshold, Crypto* cryptobj){

    // Create two vectors to store encrypted "bit-by-bit" algorithms: (<) and (=)
    // less: ( ctxt_input_i < ctxt_threshold_i) = (NOT ctxt_input_i) AND ctxt_threshold_i
    //  eq:  ( ctxt_input_i = ctxt_threshold_i) =  ctxt_input_i XNOR ctxt_threshold_i

    int vec_len = ctxt_input.size();
    vector<Ctxt> ctxt_bit_less;
    vector<Ctxt> ctxt_bit_eq;
    vector<Ctxt> ctxt_bit_comparison;


    for (int i = 0; i<vec_len; i++){
        ctxt_compare = this->NOT(ctxt_input[i]);
        ctxt_compare = this->AND(ctxt_compare, ctxt_threshold[i]);        // (NOT  ctxt_input[i]) * ctxt_threshold[i]
        ctxt_bit_less.push_back(ctxt_compare);
        if (i>0){
            ctxt_compare = this->XNOR(ctxt_compare, ctxt_input[i], ctxt_threshold[i]);
            ctxt_bit_eq.push_back(ctxt_compare);
        }
    }

    //DEBUG
    ZZX result;
    cout << "ctxt_bit_less length: " << ctxt_bit_less.size() << endl;
    cout << "Printing ctxt_bit_less encrypted content:" << endl;
    for (int i = 0; i < ctxt_bit_less.size(); i++ ){
        cryptobj->decrypt(result, ctxt_bit_less[i]);
        cout << result[0] << "  ";
    }

    cout << endl;

    cout << "ctxt_bit_eq length: " << ctxt_bit_eq.size() << endl;
    cout << "Printing ctxt_bit_eq encrypted content:" << endl;
    for (int i = 0; i < ctxt_bit_eq.size(); i++ ){
        cryptobj->decrypt(result, ctxt_bit_eq[i]);
        cout << result[0] << "  ";
    }

    cout << endl;


    // Execute comparison circuit:

    // Computing the inner AND operations
    ctxt_bit_comparison.push_back(ctxt_bit_less[vec_len-1]);

    ctxt_compare = this->AND(ctxt_bit_eq[vec_len-2], ctxt_bit_less[vec_len-2]);
    ctxt_bit_comparison.push_back(ctxt_compare);

    for (int i=vec_len-3; i>-1; i--)
    {
        ctxt_compare = ctxt_bit_eq[vec_len-2];        // x_l = y_l
        for (int j = vec_len-2; j>i; j--)
        {
            ctxt_compare = this->AND(ctxt_compare, ctxt_bit_eq[j-1]);
        }
        ctxt_compare = this->AND(ctxt_compare, ctxt_bit_less[i]);
        ctxt_bit_comparison.push_back(ctxt_compare);
    }

    //DEBUG
    cout << "ctxt_bit_comparison length: " << ctxt_bit_comparison.size() << endl;
    for (int k = 0; k < ctxt_bit_comparison.size(); k++) {
        cryptobj->decrypt(result, ctxt_bit_comparison[k]);
        cout << result[0] << "  ";
    }

    // Computing the outer OR operations

    ctxt_compare = ctxt_bit_comparison[0];
    for (int i=1; i<vec_len; i++)
    {
        ctxt_compare = this->OR(ctxt_compare, ctxt_compare, ctxt_bit_comparison[i]);
    }

    return ctxt_compare;
}

/*
 * TreeEval.h
 *
 *      Author: asma
 */

#ifndef LNV2011_TREEEVAL_TREEEVAL_H_
#define LNV2011_TREEEVAL_TREEEVAL_H_

#include <NTL/ZZ_pX.h>
#include "../HEctxt.h"
#include <vector>
#include "../HEcrypto.h"
#include "../HEkeygen.h"
#include "../HEpubkey.h"
#include <common/timing.h>
#include "node.h"

void printZZXVector(string name, const vector<ZZX> v);

char *toBinary(int number);
char *toBinary(int number, int bit_length);

void printBits (char *bits);

char *reverseBinary(char *Binary);

vector<node> StructTree(int height, string type);

int EvaluateTreePlain(node* n);
HEctxt *EvaluateTree(node* n, bool he, HEctxt *one, HEcrypto *he_crypto, vector<HEpubkey *> he_homomorphismKeys);

void test_tree_plaintxt(int height, string type);
void run_HEtree_test(int height, string type, LNV2011Params *params, HEcrypto* he_crypto, HEkeygen *he_keygen, HEpubkey *he_pubkey, const ZZ_pX sk, vector<HEpubkey *> he_homomorphismKeys, timing *timer);
void run_tree_example(); //initial test
#endif /* LNV2011_TREEEVAL_TREEEVAL_H_ */

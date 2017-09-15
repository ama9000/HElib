//
// Created by ph on 9/14/17.
//

#ifndef SECUREDECISIONTREE_UTIL_H
#define SECUREDECISIONTREE_UTIL_H

#include <string>
#include <sys/stat.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>


using namespace std;
using namespace NTL;



// a symbolic variable for printing polynomials; no actual computational meaning
const std::string syms = "x";

#define LENGTH_OF(x) (sizeof(x)/sizeof(*x))

enum RUN_TYPE {TERMINAL, ECLIPSE, RERUN};


struct Options {
    string scheme_params_filename="";
    RUN_TYPE rType=ECLIPSE;
    int terminal_run_count=1;
    string rerun_json_filename="";
};

struct PerformanceStats {
    bool success=false;
    int encrypt_successed=0, encrypt_failed=0;
    int addition_successed=0, addition_failed=0;
    int subtraction_succeeded=0, subtraction_failed=0;
    int multiplication_successed=0, multiplication_failed=0;
    int multiplication_relin_successed=0, multiplication_relin_failed=0;
};



inline bool fileExists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}





/* math class */
class Math {
private:

public:



};








/* helper class */
class Helper {
private:
    static void printSeparatorWithTitle(const string title, const char marker);



public:
    static void parseOptions(Options*, int, char**);
    static long getRandomInt(int, int);

    static char* intToBinary(unsigned int);

    /* print functions */
    static void debugMSG(string);
    static void sysMSG(string);
    static void errorMSG(string);

    static void printInt(string, int);
    static void printString(string, string);
    static void printDouble(string, double);

    static long double_To_long(const double value_double);
    static string ZZToString(const ZZ z);
    static string ZZ_pToString(const ZZ_p z);
    static int log_baseT(const ZZ baseT, const ZZ x);
    static int log_baseT(int baseT, long x);
    static void printBaseTvector(const vector<ZZ> coeffsInBaseT, const ZZ q, const ZZ t);
    static void printCoeffSamples(vector<int> coeffSamples);
    static void printZZX(string name, const ZZX poly);
    static void printZZX(const ZZX poly);
    static void printZZ_pX(string name, const ZZ_pX poly);
    static void printZZ_pX(const ZZ_pX poly);
    static void printZZ_pX_line(string name, const ZZ_pX poly);
    static void printZZ_pX_line(const ZZ_pX poly);
    static void printZZ(string name, const ZZ n);
    static void printZZ(const ZZ n);
    static void printZZ_p(string name, const ZZ_p n);
    static void printZZ_p(const ZZ_p n);
    static void printINT(string name, const long n);
    static void printINT(const long n);
    static void printDOUBLE(string name, const double n);
    static void printDOUBLE(const double n);
    static void printHeader(const string title);
    static void printSubHeader(const string title);
    static void spinner(int spin_seconds);
    static void printPStats(PerformanceStats *pStats);
    static void printVectorOfLong(string name, vector<long> v);
    static void printVectorOfZZ(string name, vector<ZZ> v);
    static void printZZ_pXVector(string name, const vector<ZZ_pX> v);
    static void printZZ_pXBinVector(string name, const vector<ZZ_pX> v);
    static string returnStringFromZZ_pX(const ZZ_pX poly);

};



#endif //SECUREDECISIONTREE_UTIL_H

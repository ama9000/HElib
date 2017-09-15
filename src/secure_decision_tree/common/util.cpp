//
// Created by ph on 9/14/17.
//

#include "util.h"

#include <random>
#include <math.h>
#include <fstream>
#include <string>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>
#include <unistd.h>


using namespace std;

/* math class functions */



/* helper class functions */


void help_page(char **argv) {
    cout << "Usage: " << argv[0] << "[options]\n"
            "\nOptions:\n"
            "-h	        Print this message and exit\n"
            "-C [filepath]  \tRun a test with given parameters\n"
            "-R [filepath]  \tRerun a test with previous parameters\n"
            //			"-S [scheme]	\tSpecify a scheme to run the demo\n"
            "-T [NumOfRun]  \tRun multiple tests just like in terminal\n"
            "\n";
}


void Helper::parseOptions(Options *opts, int argc, char** argv){
    int opt;

    // Shut GetOpt error messages down (return '?'):
    opterr = 0;

    // Retrieve the options:
    while ( (opt = getopt(argc, argv, "hC:R:T:")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'h':
                help_page(argv);
                abort();
            case 'C':   // specific scheme configurations
                opts->scheme_params_filename=optarg;
                break;
            case 'R':   // rerun the experiment with given fixed parameters
                opts->rType=RERUN;
                opts->rerun_json_filename=optarg;
                break;
                //		case 'S':
                //			if(strcmp(optarg,"LNV2011")==0)
                //				opts->scheme=LNV2011;
                //			else if(strcmp(optarg,"YASHE")==0)
                //				opts->scheme=YASHE;
                //			else{
                //				cerr<< "Select a supported scheme to run." << endl;
                //				abort();
                //			}
                //break;
            case 'T':
                opts->rType=TERMINAL;
                if(isdigit(optarg[0])){
                    opts->terminal_run_count=atoi(optarg);
                }else{
                    abort();
                }
                break;
            case '?':
                if (optopt == 'T' || optopt == 'R'|| optopt == 'C')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
            default:
                abort();
        }
    }
}


long Helper::getRandomInt(int low_bound, int high_bound){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long> dis(low_bound, high_bound);
    return dis(gen);
}



void Helper::debugMSG(string msg) {
    cout << "[DEBUG] " << msg << endl;
}

void Helper::sysMSG(string msg) {
    cout << "[SYS] " << msg << endl;
}

void Helper::errorMSG(string msg) {
    cout << "[ERROR] " << msg << endl;
}

void Helper::printInt(string name, int value) {
    cout << name << ": " << value << endl;
}

void Helper::printString(string name, string value) {
    cout << name << ": " << value << endl;
}

void Helper::printDouble(string name, double value) {
    cout << name << ": " << value << endl;
}

char* Helper::intToBinary(unsigned int dec){
    string binary = "";
    if(dec == 0){
        binary = "0";
    } else {
        while (dec > 0) {
            binary += std::to_string(dec%2);
            dec = dec / 2;
        }
    }
    char* bin = new char[binary.length()];
    strcpy(bin, binary.c_str());
    return bin;
}


long Helper::double_To_long(const double value_double){
    return static_cast<long>(value_double);
}

string Helper::ZZToString(const ZZ z) {
    stringstream buffer;
    buffer << z;
    return buffer.str();
}

string Helper::ZZ_pToString(const ZZ_p z) {
    stringstream buffer;
    buffer << z;
    return buffer.str();
}

int Helper::log_baseT(const ZZ baseT, const ZZ x) {
    return (int)(log(x) / log(baseT));
}

int Helper::log_baseT(const int baseT, const long x) {
    return (int)(log(x) / log(baseT));
}

void Helper::printBaseTvector(const vector<ZZ> coeffsInBaseT, const ZZ q, const ZZ t){
#ifdef VERBOSE
    int v_size= log_baseT(t, q)+1;
	cout << "0x";
	for(int j=0;j<v_size;j++)
		cout << to_long(coeffsInBaseT[j]) << " ";
	cout << endl;
#endif
}

void Helper::printCoeffSamples(vector<int> coeffSamples){
#ifdef VERBOSE
    for(int j=0;j<coeffSamples.size();++j){
		cout << coeffSamples[j] << " ";
	}
	cout << endl;
#endif
}

void Helper::printVectorOfLong(string name, vector<long> v){
    cout<<"List of " <<name << ":";
    for(int i=0;i<v.size();i++){
        cout<< v[i]<<" ";
    }
    cout<<endl;
}

void Helper::printVectorOfZZ(string name, vector<ZZ> v){
    cout<<"List of " <<name << ":";
    for(int i=0;i<v.size();i++){
        cout<< v[i]<<" ";
    }
    cout<<endl;
}

// Be careful about the sizeof(f) always give 8, but deg(f) reflect the actual vector size
// incorrect use will cause cout misbehavior
void Helper::printZZX(string name, const ZZX poly){
#ifdef INFO
    string output;
	//cout << sizeof(poly) << " " << deg(poly) << endl;
	for(int i=0; i<deg(poly)+1; i++){
		if(poly[i]!=0){
			// addition sign
			if(output.length()>0 && poly[i] >0) output.append(" + ");

			if (poly[i]!=1 || i==0) output.append(ZZToString(poly[i]));

			if(i>0) output.append(syms).append("^").append(to_string(i));
		}
	}

	if (name.length()>0)
		cout << name << "=" << output << endl;
	else
		cout << output << endl;
#endif
}


void Helper::printZZX(const ZZX poly){
    printZZX("", poly);
}


void Helper::printZZ_pX(string name, const ZZ_pX poly){
#ifdef INFO
    string output;
	//cout << sizeof(poly) << " " << deg(poly) << endl;
	for(int i=0; i<deg(poly)+1; i++){
		if(poly[i]!=0){
			// addition sign
			if(output.length()>0 && (conv<ZZ>(poly[i]) >0)) output.append(" + ");

			if (poly[i]!=1 || i==0) output.append(ZZ_pToString(poly[i]));

			if(i>0) output.append(syms).append("^").append(to_string(i));
		}
	}
	if (output == "")
	{
		output.append("0");
	}

	if (name.length()>0)
		cout << name << "=" << output << endl;
	else
		cout << output << endl;
#endif
}

void Helper::printZZ_pX_line(string name, const ZZ_pX poly){
#ifdef INFO
    string output;
	//cout << sizeof(poly) << " " << deg(poly) << endl;
	for(int i=0; i<deg(poly)+1; i++){
		if(poly[i]!=0){
			// addition sign
			if(output.length()>0 && (conv<ZZ>(poly[i]) >0)) output.append(" + ");

			if (poly[i]!=1 || i==0) output.append(ZZ_pToString(poly[i]));

			if(i>0) output.append(syms).append("^").append(to_string(i));
		}
	}
	if (output == "")
	{
		output.append("0");
	}

	if (name.length()>0)
		cout << name << "=" << output;
	else
		cout << output;
#endif
}

string Helper::returnStringFromZZ_pX(const ZZ_pX poly){
    string output;
    //cout << sizeof(poly) << " " << deg(poly) << endl;
    for(int i=0; i<deg(poly)+1; i++){
        if(poly[i]!=0){
            // addition sign
            if(output.length()>0 && (conv<ZZ>(poly[i]) >0)) output.append(" + ");

            if (poly[i]!=1 || i==0) output.append(ZZ_pToString(poly[i]));

            if(i>0) output.append(syms).append("^").append(to_string(i));
        }
    }
    return output;
}

void Helper::printZZ_pX(const ZZ_pX poly){
    printZZ_pX("", poly);
}

void Helper::printZZ_pX_line(const ZZ_pX poly){
    printZZ_pX_line("", poly);
}

void Helper::printZZ_pXVector(string name, const vector<ZZ_pX> v){
    cout<<endl;
    cout<< name << "(" << v.size() << ")" <<endl;
    for(int i=0;i<v.size();i++){
        printZZ_pX(v[i]);
    }
}

void Helper::printZZ_pXBinVector(string name, const vector<ZZ_pX> v){
    cout<< name << "(" << v.size() << ")" <<endl;
    for(int i=0;i<v.size();i++){
        printZZ_pX_line(v[i]);
    }
    cout << endl;
}


void Helper::printZZ_p(string name, const ZZ_p n){
#ifdef INFO
    if (name.length()>0)
		cout << name << "=" << n << endl;
	else
		cout << n << endl;
#endif
}

void Helper::printZZ_p(const ZZ_p n){
    printZZ_p("", n);
}


void Helper::printZZ(string name, const ZZ n){
#ifdef INFO
    if (name.length()>0)
		cout << name << "=" << n << endl;
	else
		cout << n << endl;
#endif
}

void Helper::printZZ(const ZZ n){
    printZZ("", n);
}


void Helper::printINT(string name, const long n){
#ifdef INFO
    if (name.length()>0)
		cout << name << "=" << n << endl;
	else
		cout << n << endl;
#endif
}

void Helper::printINT(const long n){
    printINT("", n);
}

void Helper::printDOUBLE(string name, const double n){
#ifdef INFO
    if (name.length()>0)
		cout << name << "=" << n << endl;
	else
		cout << n << endl;
#endif
}

void Helper::printDOUBLE(const double n){
    printDOUBLE("", n);
}

void Helper::printSeparatorWithTitle(const string title, const char marker){
#ifdef INFO
    cout << endl;
	for(int i=0;i<=10;i++)
		cout << marker;
	cout <<" " << title << " " ;
	for(int i=0;i<=10;i++)
		cout << marker;
	cout<<endl;
#endif
}

void Helper::printHeader(const string title){
    printSeparatorWithTitle(title, '=');
}

void Helper::printSubHeader(const string title){
    printSeparatorWithTitle(title, '-');
}
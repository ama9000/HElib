//
// Created by ph on 9/14/17.
//

#include "util.h"

#include <random>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

/* math class functions */



/* helper class functions */

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

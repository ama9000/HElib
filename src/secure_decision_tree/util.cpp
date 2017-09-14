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

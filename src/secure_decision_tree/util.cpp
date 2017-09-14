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

long helper::getRandomInt(int low_bound, int high_bound){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long> dis(low_bound, high_bound);
    return dis(gen);
}



void helper::debugMSG(string msg) {
    cout << "[DEBUG] " << msg << endl;
}

void helper::sysMSG(string msg) {
    cout << "[SYS] " << msg << endl;
}

void helper::errorMSG(string msg) {
    cout << "[ERROR] " << msg << endl;
}

void helper::printInt(string name, int value) {
    cout << name << ": " << value << endl;
}

void helper::printString(string name, string value) {
    cout << name << ": " << value << endl;
}

void helper::printDouble(string name, double value) {
    cout << name << ": " << value << endl;
}
//
// Created by ph on 9/14/17.
//


#include <random>
#include <math.h>
#include <iostream>
#include <fstream>




int static GetRandomInt(int low_bound, int high_bound){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> dis(low_bound, high_bound);
    return dis(gen);
}
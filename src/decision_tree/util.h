//
//  util.h
//  
//
//  Created by Asma Aloufi
//
//

#ifndef util_h
#define util_h

#include <cstddef>
#include <sys/time.h>


typedef unsigned int uint;
typedef unsigned long ulong;

class Timer
{
public:
    void start() { m_start = my_clock(); }
    void stop() { m_stop = my_clock(); }
    double elapsed_time() const {
        return m_stop - m_start;
    }
    
private:
    double m_start, m_stop;
    double my_clock() const {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
};


ZZX createPolyFromCoeffsArray(char *coeffs);
char *toBinary(int number);
char *toBinary(int number, int bit_length);
void printBits(char *bits);
void printZZX(ZZX poly);


#endif /* util_h */

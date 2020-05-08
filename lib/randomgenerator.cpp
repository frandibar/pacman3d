#include "randomgenerator.h"

#include <cstdlib>
#include <ctime>

const int RandomGenerator::RANDOM_MAX = 32767;

//entre 0 y RANDOM_MAX 
int RandomGenerator::get() 
{
    srand(time(0));
    return rand();
}

//flotante entre 0 y 1
double RandomGenerator::getf() 
{
    srand(time(0));
    return (double)(rand()/RANDOM_MAX);
}

//entre rango
int RandomGenerator::getRange(int min, int max) 
{
    srand(time(0));
    int i = min;
    int j = rand();
    j = j%(max - min + 1);
    return i + j;
}

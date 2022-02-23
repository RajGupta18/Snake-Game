#include "random.h"

unsigned int Random::xn = 21;
unsigned int Random::a = 4;
unsigned int Random::c = 1;

void Random::setSeed(unsigned int s) {
    xn = s;
}

unsigned int Random::rand(unsigned int m) {
    unsigned int temp = (a*xn + c)%m;
    xn = temp;
    return temp;
}
#ifndef RANDOM_H
#define RANDOM_H

class Random {
private:
    static unsigned int xn;
    static unsigned int a,c;

public:
    static void setSeed(unsigned int s);
    static unsigned int rand(unsigned int m);
};
#endif
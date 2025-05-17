#ifndef PROJECT_MINESWEEPER_RANDOM_H
#define PROJECT_MINESWEEPER_RANDOM_H

#include <random>
#include <ctime>

class Random {
//    static variables exist for the lifetime of your program. You can't make duplicates. they are shared among all objects.
//    To access static varaibles within a nonstatic function we can use the scope resolution operator :: such as Random::VarX.
    static std::mt19937 random;
public:
    static int Int(int min, int max); //this-> does not exist within static functions. We can only access static variables.
    static float Float(float min, float max);
};


#endif

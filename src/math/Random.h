#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
    public:
        static void Init()
        {
            isInit = true;
            RandomEngine.seed(std::random_device()());
            std::srand(849842);
        }

        static float Float01()
        {
           return (float)rand() / (float)RAND_MAX;
        }

        static float Floatm11()
        {
            return Float01()*2-1;
        }

        static float Angle()
        {
            return 3.1415926535f*Floatm11();
        }

    private:
        static bool isInit;
        static std::mt19937 RandomEngine;
        static std::uniform_int_distribution<std::mt19937::result_type> Distribution;
};

#endif

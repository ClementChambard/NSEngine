#include "Random.h"

bool Random::isInit = false;
std::mt19937 Random::RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::Distribution;

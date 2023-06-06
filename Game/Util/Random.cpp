#include "Random.h"

#include <random>

namespace Utils
{
    float Random::GetRandomFloat(float min, float max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);

        return dis(gen);
    }

    int Random::GetRandomInt(int min, int max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min, max);

        return dis(gen);
    }
}
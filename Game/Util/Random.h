#ifndef UTILS_RANDOM
#define UTILS_RANDOM

#include <Singleton.h>

namespace Utils
{
    class Random final : public Engine::Singleton<Random>
    {
    public:
        float GetRandomFloat(float min, float max);
        int GetRandomInt(int min, int max);

    private:
        friend class Engine::Singleton<Random>;
        Random() = default;
        ~Random() = default;
    };
}

#endif // UTILS_RANDOM
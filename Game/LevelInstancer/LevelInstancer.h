#ifndef LEVEL_INSTANCER_H
#define LEVEL_INSTANCER_H

#include <vector>
#include "LevelLoader/Structs/Level.h"
#include "LevelLoader/Structs/GameSettings.h"

namespace Galaga
{
    class LevelInstancer final
    {
    public:
        LevelInstancer(std::vector<Level>& levels, const GameSettings& gameSettings);
        // will receive level and game settings and make the levels.

    private:
        void CreateLevel(Level& level);

        GameSettings m_GameSettings{};
    };
}

#endif // LEVEL_INSTANCER
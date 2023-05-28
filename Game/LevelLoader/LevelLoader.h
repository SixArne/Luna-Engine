#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

namespace Galaga
{
    class LevelLoader
    {
    public:
        LevelLoader() = default;
        ~LevelLoader() = default;

        void LoadLevel(const std::string& levelName);
    };
}

#endif // !LEVEL_LOADER_H
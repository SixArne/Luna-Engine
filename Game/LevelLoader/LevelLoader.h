#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <string>
#include <rapidjson/document.h>

#include "Structs/GameSettings.h"
#include "Structs/Level.h"

namespace Galaga
{
    class LevelLoader final
    {
    public:
        LevelLoader() = default;
        ~LevelLoader() = default;

        void LoadGameSettings(const std::string& fileName, GameSettings& settings);
        void LoadLevel(const std::string& levelName, Level& level);

    private:
        void ReadJsonFile(const std::string& fileName, rapidjson::Document& document);
        int ReadObject(const std::string& fieldName, const std::string& subFieldName, rapidjson::Document& document);

        std::string m_CurrentFileName{};
    };
}

#endif // !LEVEL_LOADER_H
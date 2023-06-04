#include "LevelLoader.h"
#include <rapidjson/document.h>
#include <fstream>
#include <Core/Log.h>

void Galaga::LevelLoader::LoadGameSettings(const std::string& fileName, GameSettings& settings)
{
    rapidjson::Document document{};
    ReadJsonFile(fileName, document);

    // Points
    settings.points.BEE_DIVING =                    ReadObject("points", "bee_diving", document);
    settings.points.BEE_IN_FORMATION =              ReadObject("points", "bee_in_formation", document);
    settings.points.BUTTERFLY_DIVING =              ReadObject("points", "butterfly_diving", document);
    settings.points.BUTTERFLY_IN_FORMATION =        ReadObject("points", "butterfly_in_formation", document);
    settings.points.BOSS_GALAGA_DIVING =            ReadObject("points", "boss_galaga_diving", document);
    settings.points.BOSS_GALAGA_IN_FORMATION =      ReadObject("points", "boss_galaga_in_formation", document);

    // Enemy health
    settings.enemyHealth.BEE =                      ReadObject("enemy_health", "bee", document);
    settings.enemyHealth.BUTTERFLY =                ReadObject("enemy_health", "butterfly", document);
    settings.enemyHealth.BOSS_GALAGA =              ReadObject("enemy_health", "boss_galaga", document);

    // Enemy speed
    settings.enemySpeed.BEE =                       ReadObject("enemy_speed", "bee", document);
    settings.enemySpeed.BUTTERFLY =                 ReadObject("enemy_speed", "butterfly", document);
    settings.enemySpeed.BOSS_GALAGA =               ReadObject("enemy_speed", "boss_galaga", document);

    // Player settings
    settings.playerSettings.HEALTH =                ReadObject("player", "health", document);
    settings.playerSettings.BULLETS_PER_SCREEN =    ReadObject("player", "bullets_per_screen", document);
}

void Galaga::LevelLoader::LoadLevel(const std::string& fileName, Level& level)
{
    rapidjson::Document document{};
    ReadJsonFile(fileName, document);

    if (document.HasMember("name") && document["name"].IsString())
    {
        auto name = document["name"].GetString();
        level.name = name;
    }

    if (document.HasMember("rows") && document["rows"].IsInt())
    {
        auto rows = document["rows"].GetInt();
        level.rows = rows;
    }

    // Boss spawn data
    level.boss.row =            ReadObject("boss", "row", document);
    level.boss.row_span =       ReadObject("boss", "row-span", document);
    level.boss.count =          ReadObject("boss", "count", document);
    level.boss.space_between =  ReadObject("boss", "space_between", document);

    // Butterfly spawn data
    level.butterfly.row =            ReadObject("butterfly", "row", document);
    level.butterfly.row_span =       ReadObject("butterfly", "row-span", document);
    level.butterfly.count =          ReadObject("butterfly", "count", document);
    level.butterfly.space_between =  ReadObject("butterfly", "space_between", document);

    // Bee spawn data
    level.bee.row =            ReadObject("bee", "row", document);
    level.bee.row_span =       ReadObject("bee", "row-span", document);
    level.bee.count =          ReadObject("bee", "count", document);
    level.bee.space_between =  ReadObject("bee", "space_between", document);
}

void Galaga::LevelLoader::ReadJsonFile(const std::string& fileName, rapidjson::Document& document)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        L_ERROR("Couldn't open file")
        return;
    }

    std::string jsonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    document.Parse(jsonData.c_str());

    if (document.HasParseError())
    {
        L_ERROR("Error whilst parsing document");
        return;
    }

    m_CurrentFileName = fileName;

    file.close();
}

int Galaga::LevelLoader::ReadObject(const std::string& fieldName, const std::string& subFieldName, rapidjson::Document& document)
{
    const char* field = fieldName.c_str();
    bool hasField = document.HasMember(field);

    if (hasField && document[field].IsObject())
    {
        const rapidjson::Value& points = document[field];
        const char* subfield = subFieldName.c_str();

        if (points.HasMember(subfield) && points[subfield].IsInt()) {
            int value = points[subfield].GetInt();
            return value;
        }
    }

    L_ERROR("Error in file: {} with field: {} subfield: {}", m_CurrentFileName, fieldName, subFieldName);
    return -1;
}
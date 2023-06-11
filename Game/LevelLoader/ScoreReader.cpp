#include "ScoreReader.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Galaga
{
    std::vector<Score> ScoreReader::ReadScores(const std::string& filename)
    {
        m_CurrentFileName = filename;
        std::ifstream fileStream(filename.c_str());
        rapidjson::IStreamWrapper isw(fileStream);

        rapidjson::Document doc;
        doc.ParseStream(isw);

        std::vector<Score> scores;

        if(doc.IsArray())
        {
            for (auto& v : doc.GetArray())
            {
                Score result;
                result.name = v["name"].GetString();
                result.score = v["score"].GetInt();
                scores.push_back(result);
            }
        }
        return scores;
    }
}
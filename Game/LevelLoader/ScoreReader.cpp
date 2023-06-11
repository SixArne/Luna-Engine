#include "ScoreReader.h"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

namespace Galaga
{
    std::vector<int> ScoreReader::ReadScores(const std::string& filename)
    {
        m_CurrentFileName = filename;
        std::ifstream fileStream(filename.c_str());
        rapidjson::IStreamWrapper isw(fileStream);

        rapidjson::Document doc;
        doc.ParseStream(isw);

        std::vector<int> scores;

        if(doc.IsArray())
        {
            for (auto& v : doc.GetArray())
            {
                int score = v["score"].GetInt();
                scores.push_back(score);
            }
        }
        return scores;
    }
}
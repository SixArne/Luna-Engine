#include "ScoreWriter.h"

#include <string>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include "ScoreReader.h"

namespace Galaga
{
    void ScoreWriter::WriteScores(const std::string& fileName, int newScore)
    {
        ScoreReader scoreReader{};
        auto scores = scoreReader.ReadScores(fileName);
        scores.push_back({"", newScore});

        m_CurrentFileName = fileName;
        rapidjson::Document doc;
        doc.SetArray();

        for (auto& score : scores)
        {
            rapidjson::Value obj(rapidjson::kObjectType);
            obj.AddMember("name", rapidjson::Value(score.name.c_str(), doc.GetAllocator()), doc.GetAllocator());
            obj.AddMember("score", score.score, doc.GetAllocator());
            doc.PushBack(obj, doc.GetAllocator());
        }

        std::ofstream fileStream(fileName.c_str());
        rapidjson::OStreamWrapper osw(fileStream);
        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
        doc.Accept(writer);
    }
}
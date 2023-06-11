#ifndef SCORE_READER_H
#define SCORE_READER_H

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include "Structs/Score.h"

namespace Galaga
{
    class ScoreReader final
    {
    public:
        ScoreReader() = default;
        ~ScoreReader() = default;

        std::vector<int> ReadScores(const std::string& fileName);

    private:


        std::string m_CurrentFileName{};
    };
}

#endif // !SCORE_READER_H
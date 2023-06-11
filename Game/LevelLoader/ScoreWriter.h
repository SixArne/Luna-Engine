#ifndef SCORE_WRITER_H
#define SCORE_WRITER_H

#include <string>
#include <vector>
#include "Structs/Score.h"
#include <rapidjson/document.h>

namespace Galaga
{
    class ScoreWriter final
    {
    public:
        ScoreWriter() = default;
        ~ScoreWriter() = default;

        void WriteScores(const std::string& fileName, const std::vector<Score>& scores);

    private:
        std::string m_CurrentFileName{};
    };
}

#endif // !SCORE_WRITER_H
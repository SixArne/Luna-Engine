#ifndef GALAGA_UTILS_BEZIER_SPLINE_H
#define GALAGA_UTILS_BEZIER_SPLINE_H

#include <vector>
#include <glm/vec2.hpp>

namespace Utils
{
    class BezierSpline
    {
    public:
        BezierSpline() = default;
        BezierSpline(std::vector<glm::vec2>& points);
        glm::vec2 GetTravellersPosition() const;

        void Update(float t);

    private:
        std::vector<glm::vec2> m_Points{};
        glm::vec2 m_TravellerPosition{};
        glm::vec2 m_Tangent{};
        glm::vec2 m_Velocity{};
        glm::vec2 m_Normal{};
    };
}

#endif  // GALAGA_UTILS_BEZIER_H

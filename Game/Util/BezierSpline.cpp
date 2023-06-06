#include "BezierSpline.h"

#include <glm/glm.hpp>

namespace Utils
{
    BezierSpline::BezierSpline(std::vector<glm::vec2>& points)
    {
        for (auto& pos: points)
        {
            m_Points.push_back(pos);
        }
    }

    glm::vec2 BezierSpline::GetTravellersPosition() const
    {
        return m_TravellerPosition;
    }

    void BezierSpline::Update(float t)
    {
        const float cubedTime = t * t * t;
        const float squaredTime = t * t;

        const glm::vec2& p0 = m_Points[0];
        const glm::vec2& p1 = m_Points[1];
        const glm::vec2& p2 = m_Points[2];
        const glm::vec2& p3 = m_Points[3];

        m_TravellerPosition =
            p0 * (-cubedTime + (3.0f * squaredTime) - (3.0f * t) + 1) +
            p1 * ((3.0f * cubedTime) - (6.0f * squaredTime) + (3.0f * t)) +
            p2 * ((-3.0f * cubedTime) + (3.0f * squaredTime)) +
            p3 * (cubedTime);

        m_Velocity =
            p0 * ((-3.0f * squaredTime) + (6.0f * t) - 3.0f) +
            p1 * ((9.0f * squaredTime) - (12.0f * t) + 3.0f) +
            p2 * ((-9.0f * squaredTime) + (6.0f * t)) +
            p3 * (3.0f * squaredTime);

        m_Tangent = glm::normalize(m_Velocity);

        m_Normal = glm::vec2{ -m_Tangent.y, m_Tangent.x };
    }
}
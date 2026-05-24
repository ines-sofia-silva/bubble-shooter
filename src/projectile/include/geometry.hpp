#pragma once

#include <cstddef>

namespace ProjectileGeometry
{
inline constexpr double kPi = 3.14159265358979323846;
inline constexpr double kCollisionRadius = 0.48;

inline double cellCenterX(std::size_t row, std::size_t col)
{
    return static_cast<double>(col) + ((row % 2U) != 0U ? 0.5 : 0.0);
}

inline double cellCenterY(std::size_t row)
{
    return static_cast<double>(row);
}

inline double toRadians(double angleDegrees)
{
    return angleDegrees * (kPi / 180.0);
}
} // namespace ProjectileGeometry

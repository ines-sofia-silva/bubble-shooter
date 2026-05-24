#pragma once

#include "board.hpp"

#include <optional>

struct ProjectileState
{
    double x;
    double y;
    double vx;
    double vy;
    int bounceCount;
};

class ProjectileTrajectory
{
public:
    /**
     * @brief Computes the initial projectile state for a given angle.
     * @param board The game board.
     * @param angleDegrees The launch angle in degrees.
     * @return Optional initial state; std::nullopt if angle is invalid.
     */
    static std::optional<ProjectileState> start(const Board &board, double angleDegrees);

    /**
     * @brief Advances the projectile state by one step, handling wall bounces.
     * @param board The game board.
     * @param state The projectile state to update.
     * @return True if the projectile is still in play, false if max bounces exceeded.
     */
    static bool advance(const Board &board, ProjectileState &state);
};

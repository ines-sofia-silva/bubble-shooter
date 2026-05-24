#include <gtest/gtest.h>
#include "trajectory.hpp"
#include "board.hpp"

class ProjectileTrajectoryTest : public ::testing::Test {
protected:
    Board board{8, 8, 4};
};

TEST_F(ProjectileTrajectoryTest, StartRejectsHorizontalOrDownward) {
    // 0 degrees (horizontal right)
    EXPECT_FALSE(ProjectileTrajectory::start(board, 0.0).has_value());
    // 180 degrees (horizontal left)
    EXPECT_FALSE(ProjectileTrajectory::start(board, 180.0).has_value());
    // 200 degrees (downward)
    EXPECT_FALSE(ProjectileTrajectory::start(board, 200.0).has_value());
}

TEST_F(ProjectileTrajectoryTest, StartAcceptsUpward) {
    // 90 degrees (straight up)
    auto state = ProjectileTrajectory::start(board, 90.0);
    ASSERT_TRUE(state.has_value());
    EXPECT_NEAR(state->vx, 0.0, 1e-8);
    EXPECT_LT(state->vy, 0.0);
}

TEST_F(ProjectileTrajectoryTest, AdvanceBouncesOffWalls) {
    auto stateOpt = ProjectileTrajectory::start(board, 45.0);
    ASSERT_TRUE(stateOpt.has_value());
    auto state = *stateOpt;
    int bounces = 0;
    while (ProjectileTrajectory::advance(board, state)) {
        if (state.bounceCount > bounces) {
            bounces = state.bounceCount;
        }
        if (bounces > 0) break;
    }
    EXPECT_GT(bounces, 0);
}

TEST_F(ProjectileTrajectoryTest, AdvanceStopsAfterMaxBounces) {
    auto stateOpt = ProjectileTrajectory::start(board, 45.0);
    ASSERT_TRUE(stateOpt.has_value());
    auto state = *stateOpt;
    state.bounceCount = 16; // kMaxBounces
    EXPECT_FALSE(ProjectileTrajectory::advance(board, state));
}
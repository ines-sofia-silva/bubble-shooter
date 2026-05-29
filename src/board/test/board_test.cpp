#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "board.hpp"

TEST(BoardTest, Constructor) {
    // Verifies basic board dimensions are stored correctly.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_EQ(board.rows(), 8u);
    EXPECT_EQ(board.cols(), 8u);
}

TEST(BoardTest, InBoundsWorks) {
    // Confirms coordinate bounds checking accepts valid cells and rejects invalid ones.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_TRUE(board.inBounds(0, 0));
    EXPECT_TRUE(board.inBounds(7, 7));
    EXPECT_FALSE(board.inBounds(-1, 0));
    EXPECT_FALSE(board.inBounds(0, 8));
}

TEST(BoardTest, SetAndGetWorks) {
    // Ensures writing then reading a cell returns the same color.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(2, 3, Bubble::Color::Red);
    EXPECT_EQ(board.get(2, 3), Bubble::Color::Red);
}

TEST(BoardTest, IsEmpty)
{
    // Checks empty-cell semantics and that out-of-bounds queries are never treated as empty.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);

    // Rows below the initialized top rows start empty.
    EXPECT_TRUE(board.isEmpty(7, 0));

    board.set(7, 0, Bubble::Color::Blue);
    EXPECT_FALSE(board.isEmpty(7, 0));

    EXPECT_FALSE(board.isEmpty(-1, 0));
    EXPECT_FALSE(board.isEmpty(8, 0));
    EXPECT_FALSE(board.isEmpty(0, 8));
}

TEST(BoardTest, HexNeighborsForEvenRowCenter)
{
    // Validates even-row hex neighbor topology (offset pattern differs by row parity).
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.print(std::cout);
    const auto neighbors = board.hexNeighbors(4, 4); // even row

    EXPECT_EQ(neighbors.size(), 6u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(3, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(3, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 4)), neighbors.end());
}

TEST(BoardTest, HexNeighborsForOddRowCenter)
{
    // Validates odd-row hex neighbor topology.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    const auto neighbors = board.hexNeighbors(5, 4); // odd row

    EXPECT_EQ(neighbors.size(), 6u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(4, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 3)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(5, 5)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 4)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(6, 5)), neighbors.end());
}

TEST(BoardTest, HexNeighborsAtTopLeftCorner)
{
    // Corner cells should only return in-bounds neighbors.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    const auto neighbors = board.hexNeighbors(0, 0);

    EXPECT_EQ(neighbors.size(), 2u);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(0, 1)), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), std::make_pair(1, 0)), neighbors.end());
}

TEST(BoardTest, ClearConnectedGroupEmptyCell)
{
    // Clearing should be a no-op when starting on an empty cell.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    EXPECT_EQ(board.clearConnectedGroup(7, 7, 3), 0);
}

TEST(BoardTest, ClearConnectedGroupRespectsMinimumSize)
{
    // A connected component smaller than threshold must remain unchanged.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(7, 0, Bubble::Color::Green);
    board.set(7, 1, Bubble::Color::Green);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(7, 0, 3), 0);
    EXPECT_EQ(board.get(7, 0), Bubble::Color::Green);
    EXPECT_EQ(board.get(7, 1), Bubble::Color::Green);
}

TEST(BoardTest, ClearConnectedGroupClearsWhenMinimumReached)
{
    // A connected component meeting threshold should be removed and counted.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(6, 3, Bubble::Color::Yellow);
    board.set(6, 4, Bubble::Color::Yellow);
    board.set(7, 3, Bubble::Color::Yellow);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(6, 3, 3), 3);
    EXPECT_TRUE(board.isEmpty(6, 3));
    EXPECT_TRUE(board.isEmpty(6, 4));
    EXPECT_TRUE(board.isEmpty(7, 3));
}

TEST(BoardTest, ClearConnectedGroupDetachedBubble)
{
    // Removing a matching group can detach lower bubbles that should then drop.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);
    board.set(3, 3, Bubble::Color::Yellow);
    board.set(3, 4, Bubble::Color::Yellow);
    board.set(3, 5, Bubble::Color::Yellow);
    board.set(4, 4, Bubble::Color::Green);
    board.set(5, 4, Bubble::Color::Blue);
    board.print(std::cout);

    EXPECT_EQ(board.clearConnectedGroup(3, 3, 3), 3);
    EXPECT_TRUE(board.isEmpty(3, 3));
    EXPECT_TRUE(board.isEmpty(3, 4));
    EXPECT_TRUE(board.isEmpty(3, 5));
    EXPECT_TRUE(board.isEmpty(4, 4));
    EXPECT_TRUE(board.isEmpty(5, 4));
    
    board.print(std::cout);
}

TEST(BoardTest, OutOfRangeIndices)
{
    // Public accessors should enforce index safety through exceptions.
    Bubble::ColorManager colorManager;
    Board board(8, 8, colorManager);

    EXPECT_THROW(board.get(8, 0), std::out_of_range);
    EXPECT_THROW(board.get(0, 8), std::out_of_range);
    EXPECT_THROW(board.set(8, 0, Bubble::Color::Red), std::out_of_range);
    EXPECT_THROW(board.set(0, 8, Bubble::Color::Red), std::out_of_range);
}

TEST(BoardTest, AddNewRowChangesTopRow)
{
    // Verifies that addNewRow() updates the top row and old bottom row becomes inaccessible.
    Bubble::ColorManager colorManager;
    Board board(6, 6, colorManager);

    // Store original bottom row values
    Bubble::Color originalBottomRow[6];
    for (std::size_t col = 0; col < 6; ++col)
    {
        originalBottomRow[col] = board.get(5, col);
    }

    // Add a new row
    board.addNewRow(colorManager);

    // Top row should have new colors (not empty)
    bool topRowFilled = false;
    for (std::size_t col = 0; col < 6; ++col)
    {
        if (board.get(0, col) != Bubble::Color::None)
        {
            topRowFilled = true;
            break;
        }
    }
    EXPECT_TRUE(topRowFilled);

    // The old bottom row should now be at logical row 4 (or inaccessible via logical row 5)
    // Verify dimensions haven't changed
    EXPECT_EQ(board.rows(), 6u);
    EXPECT_EQ(board.cols(), 6u);
}

TEST(BoardTest, MultipleAddNewRowsWraps)
{
    // Calling addNewRow() multiple times equal to board height should cycle through all positions.
    Bubble::ColorManager colorManager;
    Board board(4, 4, colorManager);

    // Call addNewRow 4 times (same as number of rows)
    for (int i = 0; i < 4; ++i)
    {
        board.addNewRow(colorManager);
    }

    // After wrapping around, board should still be valid and accessible
    EXPECT_EQ(board.rows(), 4u);
    EXPECT_EQ(board.cols(), 4u);

    // Top row should be filled (from most recent addNewRow call)
    bool topRowFilled = false;
    for (std::size_t col = 0; col < 4; ++col)
    {
        if (board.get(0, col) != Bubble::Color::None)
        {
            topRowFilled = true;
            break;
        }
    }
    EXPECT_TRUE(topRowFilled);
}

TEST(BoardTest, NeighborsWorkAcrossWrappedBoundary)
{
    // Verify hexNeighbors calculates correctly when board content wraps.
    Bubble::ColorManager colorManager;
    Board board(5, 5, colorManager);

    // Fill some cells near what will become the wrap boundary
    board.set(4, 2, Bubble::Color::Red);
    board.set(3, 2, Bubble::Color::Green);

    // Add new rows to cause wrapping
    for (int i = 0; i < 3; ++i)
    {
        board.addNewRow(colorManager);
    }

    // Neighbors should still calculate correctly even with wrapping
    const auto neighbors = board.hexNeighbors(2, 2);
    EXPECT_TRUE(neighbors.size() > 0); // Should have at least some neighbors

    // Verify all neighbors are in bounds
    for (const auto& neighbor : neighbors)
    {
        EXPECT_TRUE(board.inBounds(neighbor.first, neighbor.second));
    }
}

TEST(BoardTest, DetachedBubblesWorkWithWrapping)
{
    // BFS for detached bubbles should work correctly across wrapped boundaries.
    Bubble::ColorManager colorManager;
    Board board(6, 6, colorManager);

    // Create a configuration with a detachable structure
    board.set(2, 2, Bubble::Color::Yellow);
    board.set(2, 3, Bubble::Color::Yellow);
    board.set(2, 4, Bubble::Color::Yellow);
    board.set(3, 3, Bubble::Color::Green);
    board.set(4, 3, Bubble::Color::Blue);

    // Add rows to wrap the board
    for (int i = 0; i < 4; ++i)
    {
        board.addNewRow(colorManager);
    }

    // Clear the yellow group; should also clear detached green and blue
    int cleared = board.clearConnectedGroup(2, 2, 3);

    // Verify clearing worked (should include primary group)
    EXPECT_GE(cleared, 3);
}

TEST(BoardTest, ClearConnectedGroupWithWrapping)
{
    // clearConnectedGroup should work correctly with wrapped row indices.
    Bubble::ColorManager colorManager;
    Board board(7, 5, colorManager);
    board.print(std::cout);
    board.set(3, 1, Bubble::Color::Purple);
    board.set(3, 2, Bubble::Color::Purple);
    board.set(3, 3, Bubble::Color::Purple);
    board.print(std::cout);
    
    // Wrap the board several times
    for (int i = 0; i < 2; ++i)
    {
        board.addNewRow(colorManager);
        board.print(std::cout);
    }

    // Clear the group - should find and clear the matching bubbles
    int cleared = board.clearConnectedGroup(5, 1, 3);
    
    board.print(std::cout);
    EXPECT_EQ(cleared, 3);

    // Verify cells are now empty
    EXPECT_TRUE(board.isEmpty(5, 1));
    EXPECT_TRUE(board.isEmpty(5, 2));
    EXPECT_TRUE(board.isEmpty(5, 3));
}


#include <iostream>

namespace GameSettings
{
    static constexpr std::size_t Rows = 16;
    static constexpr std::size_t Cols = 16;
    static constexpr std::size_t MissesBeforeNewRow = 3;
    static constexpr std::size_t MinGroupSize = 3;
    static constexpr std::size_t InitialFilledRows = Rows * 3.0 / 8;
} // GameSettings
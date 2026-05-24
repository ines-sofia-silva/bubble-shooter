#pragma once

#include <cstdint>
#include <random>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <utility>

inline uint8_t randomColor( std::size_t n_colors)
{
    static std::mt19937 rng(std::random_device{}());
    double r = std::generate_canonical<double, 32>(rng);
    return static_cast<uint8_t>(1 + static_cast<int>(r * (n_colors - 1)));
}

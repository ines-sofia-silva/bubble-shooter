#ifndef GRID_UTILS_H
#define GRID_UTILS_H

#include "../../include/game_settings.hpp"
#include <cmath>

/**
 * @brief Utilities for computing the hexagonal grid layout and coordinate transforms.
 *
 * `GridUtils` centralizes grid sizing, positioning and conversions between
 * game grid coordinates and screen pixel coordinates. It keeps layout logic
 * out of the renderer and makes it reusable from other systems.
 */
class GridUtils {
public:
    /**
     * @brief Default constructor.
     */
    GridUtils() = default;

    /**
     * @brief Initialize grid layout using current window dimensions.
     * @param windowWidth Window width in pixels.
     * @param windowHeight Window height in pixels.
     */
    void init(int windowWidth, int windowHeight);

    /**
     * @brief Recalculate grid parameters for a new window size.
     * @param windowWidth New window width in pixels.
     * @param windowHeight New window height in pixels.
     */
    void calculateGridLayout(int windowWidth, int windowHeight);

    /**
     * @brief Convert integer hex-grid coordinates to screen pixel coordinates.
     * @param gridRow Row index in the grid.
     * @param gridCol Column index in the grid.
     * @param screenX Output X coordinate in pixels.
     * @param screenY Output Y coordinate in pixels.
     */
    void hexGridToScreenCoord(const int &gridRow, const int &gridCol, int &screenX, int &screenY) const;

    /**
     * @brief Convert continuous (floating) grid coordinates to screen pixels.
     * @param gridRow Row coordinate (may be fractional).
     * @param gridCol Column coordinate (may be fractional).
     * @param screenX Output X coordinate in pixels.
     * @param screenY Output Y coordinate in pixels.
     */
    void gridToScreenCoord(const double &gridRow, const double &gridCol, int &screenX, int &screenY) const;

    /** @brief Get size of a hex cell in pixels. */
    int getHexSize() const { return m_hexSize; }
    /** @brief Get left X of grid area in pixels. */
    int getGridStartX() const { return m_gridStartX; }
    /** @brief Get top Y of grid area in pixels. */
    int getGridStartY() const { return m_gridStartY; }
    /** @brief Total width of the grid area in pixels. */
    int getGridWidth() const { return m_gridWidth; }
    /** @brief Total height of the grid area in pixels. */
    int getGridHeight() const { return m_gridHeight; }
    /** @brief Launcher X coordinate in pixels. */
    int getLauncherX() const { return m_launcherX; }
    /** @brief Launcher Y coordinate in pixels. */
    int getLauncherY() const { return m_launcherY; }

private:
    int m_hexSize{0};
    int m_gridStartX{0};
    int m_gridStartY{0};
    int m_gridWidth{0};
    int m_gridHeight{0};
    int m_launcherX{0};
    int m_launcherY{0};
};

#endif // GRID_UTILS_H

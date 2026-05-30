#include "grid_utils.hpp"

void GridUtils::init(int windowWidth, int windowHeight)
{
    calculateGridLayout(windowWidth, windowHeight);

    double launcher_grid_x = (static_cast<double>(GameSettings::Cols) - 1.0) / 2.0;
    double launcher_grid_y = static_cast<double>(GameSettings::Rows) - 1.0;
    hexGridToScreenCoord(static_cast<int>(launcher_grid_y), static_cast<int>(launcher_grid_x), m_launcherX, m_launcherY);
}

void GridUtils::calculateGridLayout(int windowWidth, int windowHeight)
{
    m_hexSize = static_cast<int>(std::min(windowWidth / (GameSettings::Cols * 1.5), windowHeight / (GameSettings::Rows * 1.5)));

    int hexWidth = static_cast<int>(m_hexSize);
    int hexHeight = m_hexSize;

    m_gridWidth = hexWidth * GameSettings::Cols;
    m_gridHeight = hexHeight * GameSettings::Rows;

    m_gridStartX = (windowWidth - m_gridWidth) / 2;
    m_gridStartY = (windowHeight - m_gridHeight) / 3;
}

void GridUtils::hexGridToScreenCoord(const int &gridRow, const int &gridCol, int &screenX, int &screenY) const
{
    int hexWidth = static_cast<int>(m_hexSize * 1.1);
    int hexHeight = m_hexSize * 2;

    screenX = m_gridStartX + gridCol * hexWidth + (gridRow % 2) * hexWidth / 2;
    screenY = static_cast<int>(m_gridStartY + gridRow * (hexHeight / 2.0));
}

void GridUtils::gridToScreenCoord(const double &gridRow, const double &gridCol, int &screenX, int &screenY) const
{
    int hexWidth = static_cast<int>(m_hexSize * 1.1);
    int hexHeight = m_hexSize * 2;

    screenX = m_gridStartX + static_cast<int>(gridCol * hexWidth);
    screenY = static_cast<int>(m_gridStartY + gridRow * (hexHeight / 2.0));
}

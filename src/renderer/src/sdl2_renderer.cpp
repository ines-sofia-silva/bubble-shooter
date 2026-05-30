#include "sdl2_renderer.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "game_settings.hpp"

SDL2Renderer::SDL2Renderer()
        : m_primitives(std::make_unique<SDL2Primitives>()),
            m_gridUtils(std::make_unique<GridUtils>())
{
}

SDL2Renderer::~SDL2Renderer()
{
    shutdown();
}

bool SDL2Renderer::init(int windowWidth, int windowHeight, const char *title)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!m_window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Set renderer color and blend mode
    SDL_SetRenderDrawColor(m_renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                           BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    // wire helpers
    if (m_primitives)
        m_primitives->setRenderer(m_renderer);
    if (m_gridUtils)
        m_gridUtils->init(m_windowWidth, m_windowHeight);

    m_isRunning = true;

    return true;
}

void SDL2Renderer::initGameSettings() {
    if (m_gridUtils)
        m_gridUtils->init(m_windowWidth, m_windowHeight);
}

void SDL2Renderer::shutdown()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void SDL2Renderer::clear()
{
    if (!m_renderer)
        return;
    SDL_SetRenderDrawColor(m_renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                           BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(m_renderer);
}

void SDL2Renderer::present()
{
    if (m_renderer)
    {
        SDL_RenderPresent(m_renderer);
    }
}

// Grid calculations and primitive drawing moved to helper classes

void SDL2Renderer::render(const Board &board, const RenderStats &stats)
{
    clear();

    // Draw game board
    bool parityOffset = board.getParityOffset();
    for (std::size_t row = 0; row < GameSettings::Rows; ++row)
    {
        for (std::size_t col = 0; col < GameSettings::Cols; ++col)
        {
            int screenX, screenY;
            if (m_gridUtils)
                m_gridUtils->hexGridToScreenCoord(static_cast<int>(row) + parityOffset, static_cast<int>(col), screenX, screenY);

            Bubble::Color bubbleColor = board.get(row, col);
            SDL_Color color = m_primitives ? m_primitives->bubbleColorToSDL(bubbleColor) : SDL_Color{0,0,0,0};
            int radius = m_gridUtils ? static_cast<int>(m_gridUtils->getHexSize() / std::sqrt(3)) : 8;
            if (m_primitives)
                m_primitives->drawCircle(screenX, screenY, radius, color, true);
        }
    }


    // Draw launcher indicator
    SDL_Color next_color = m_primitives ? m_primitives->bubbleColorToSDL(stats.projectileColor) : SDL_Color{0,0,0,0};
    int screenX, screenY;
    if (m_gridUtils)
        m_gridUtils->gridToScreenCoord(stats.y, stats.x, screenX, screenY);
    int radius = m_gridUtils ? static_cast<int>(m_gridUtils->getHexSize() / std::sqrt(3)) : 8;
    if (m_primitives)
        m_primitives->drawCircle(screenX, screenY, radius, next_color, true);

    //Draw next color preview
    SDL_Color next_color_preview = m_primitives ? m_primitives->bubbleColorToSDL(stats.nextColor) : SDL_Color{0,0,0,0};
    if (m_gridUtils)
        m_gridUtils->gridToScreenCoord(stats.y, stats.x + GameSettings::Cols/2, screenX, screenY);
    if (m_primitives)
        m_primitives->drawCircle(screenX, screenY, radius, next_color_preview, true);

    // Draw aiming line if mouse is over game area
    if (m_gridUtils && m_mouseX > m_gridUtils->getGridStartX() && m_mouseX < m_gridUtils->getGridStartX() + m_gridUtils->getGridWidth() &&
        m_mouseY > 0 && m_mouseY < m_windowHeight)
    {
        renderAimingLine(calculateAngleFromMouse());
    }

    present();
}

double SDL2Renderer::calculateAngleFromMouse() const
{
    int launcherX = m_gridUtils ? m_gridUtils->getLauncherX() : 0;
    int launcherY = m_gridUtils ? m_gridUtils->getLauncherY() : 0;
    int dx = m_mouseX - launcherX;
    int dy = m_mouseY - launcherY;

    // Calculate angle in degrees (0° is straight up)
    double radians = std::atan2(dx, -dy);
    double degrees = radians * 180.0 / M_PI;

    return degrees;
}

std::optional<double> SDL2Renderer::getMouseAngle() const
{
    double angle = calculateAngleFromMouse();
    // Only return angle if within valid range (-90 to 90 degrees)
    if (angle >= -90 && angle <= 90)
    {
        return angle;
    }
    return std::nullopt;
}

void SDL2Renderer::renderAimingLine(double angle)
{

    if (!m_primitives)
        return;

    // Convert angle to radians (0° is up, increases clockwise)
    double radians = angle * M_PI / 180.0;

    // bound the angle to prevent shooting backwards
    if (radians < -M_PI / 2)
    {
        radians = -M_PI / 2;
    }
    else if (radians > M_PI / 2)
    {
        radians = M_PI / 2;
    }

    // Calculate end point of aiming line
    int length = 100;
    int launcherX = m_gridUtils ? m_gridUtils->getLauncherX() : 0;
    int launcherY = m_gridUtils ? m_gridUtils->getLauncherY() : 0;

    double endX = launcherX + length * std::sin(radians);
    double endY = launcherY - length * std::cos(radians);

    // Draw aiming line
    m_primitives->drawLine(launcherX, launcherY, endX, endY, AIMING_LINE_COLOR);
}

void SDL2Renderer::renderProjectile(const ProjectileInfo &trajectory)
{
    if (!trajectory.isValid || !m_primitives)
        return;

    // Draw trajectory line
    m_primitives->drawLine(trajectory.startX, trajectory.startY, trajectory.endX, trajectory.endY,
             SDL_Color{200, 100, 100, 128});

    // Draw projectile at final position
    m_primitives->drawCircle(static_cast<int>(trajectory.endX), static_cast<int>(trajectory.endY),
               m_gridUtils ? static_cast<int>(m_gridUtils->getHexSize() / 3) : 6, SDL_Color{200,100,100,255}, true);
}

void SDL2Renderer::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_MOUSEMOTION:
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                auto angle = getMouseAngle();
                if (angle.has_value())
                {
                    m_shouldShoot = true;
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            break;
        }
    }
}

bool SDL2Renderer::isRunning() const
{
    return m_isRunning;
}

void SDL2Renderer::requestClose()
{
    m_isRunning = false;
}

bool SDL2Renderer::shouldShoot() const
{
    return m_shouldShoot;
}

void SDL2Renderer::resetShootFlag()
{
    m_shouldShoot = false;
}

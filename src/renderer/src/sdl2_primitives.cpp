#include "sdl2_primitives.hpp"
#include <cmath>

SDL2Primitives::SDL2Primitives() = default;

void SDL2Primitives::setRenderer(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

void SDL2Primitives::drawCircle(int centerX, int centerY, int radius, const SDL_Color &color, bool filled)
{
    if (!m_renderer)
        return;

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    if (filled)
    {
        for (int w = 0; w < radius * 2; w++)
        {
            for (int h = 0; h < radius * 2; h++)
            {
                int dx = w - radius;
                int dy = h - radius;
                if ((dx * dx + dy * dy) <= (radius * radius))
                {
                    SDL_RenderDrawPoint(m_renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
    else
    {
        int x = 0;
        int y = radius;
        int d = 3 - 2 * radius;

        while (x <= y)
        {
            SDL_RenderDrawPoint(m_renderer, centerX + x, centerY + y);
            SDL_RenderDrawPoint(m_renderer, centerX - x, centerY + y);
            SDL_RenderDrawPoint(m_renderer, centerX + x, centerY - y);
            SDL_RenderDrawPoint(m_renderer, centerX - x, centerY - y);
            SDL_RenderDrawPoint(m_renderer, centerX + y, centerY + x);
            SDL_RenderDrawPoint(m_renderer, centerX - y, centerY + x);
            SDL_RenderDrawPoint(m_renderer, centerX + y, centerY - x);
            SDL_RenderDrawPoint(m_renderer, centerX - y, centerY - x);

            if (d < 0)
            {
                d = d + 4 * x + 6;
            }
            else
            {
                d = d + 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    }
}

void SDL2Primitives::drawLine(double x1, double y1, double x2, double y2, const SDL_Color &color)
{
    if (!m_renderer)
        return;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(m_renderer, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
}

void SDL2Primitives::drawRect(int x, int y, int w, int h, const SDL_Color &color, bool filled)
{
    if (!m_renderer)
        return;
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    if (filled)
    {
        SDL_RenderFillRect(m_renderer, &rect);
    }
    else
    {
        SDL_RenderDrawRect(m_renderer, &rect);
    }
}

SDL_Color SDL2Primitives::bubbleColorToSDL(Bubble::Color color) const
{
    switch (color)
    {
    case Bubble::Color::Red:
        return {255, 80, 80, 255};
    case Bubble::Color::Green:
        return {80, 255, 80, 255};
    case Bubble::Color::Blue:
        return {80, 120, 255, 255};
    case Bubble::Color::Yellow:
        return {255, 255, 80, 255};
    case Bubble::Color::Purple:
        return {200, 80, 255, 255};
    case Bubble::Color::None:
    default:
        return {0, 0, 0, 0};
    }
}

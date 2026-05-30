#ifndef SDL2_PRIMITIVES_H
#define SDL2_PRIMITIVES_H

#include <SDL2/SDL.h>
#include "bubble/include/bubble_color.hpp"

/**
 * @brief Small utility class that wraps basic SDL2 drawing primitives.
 *
 * Encapsulates drawing operations and color conversions so callers do not
 * need to manipulate the SDL_Renderer directly. This keeps rendering code
 * focused on higher-level concerns.
 */
class SDL2Primitives {
public:
    /**
     * @brief Construct a SDL2Primitives helper.
     */
    SDL2Primitives();

    /**
     * @brief Set the SDL renderer used for drawing operations.
     * @param renderer Pointer to an initialized SDL_Renderer (may be nullptr).
     */
    void setRenderer(SDL_Renderer* renderer);

    /**
     * @brief Draw a circle on the current renderer.
     * @param centerX X coordinate of the circle center in pixels.
     * @param centerY Y coordinate of the circle center in pixels.
     * @param radius Radius of the circle in pixels.
     * @param color SDL color used to draw the circle.
     * @param filled If true draws a filled circle, otherwise draws an outline.
     */
    void drawCircle(int centerX, int centerY, int radius, const SDL_Color &color, bool filled = true);

    /**
     * @brief Draw a straight line between two points.
     * @param x1 Start X in pixels.
     * @param y1 Start Y in pixels.
     * @param x2 End X in pixels.
     * @param y2 End Y in pixels.
     * @param color Color used to draw the line.
     */
    void drawLine(double x1, double y1, double x2, double y2, const SDL_Color &color);

    /**
     * @brief Draw a rectangle.
     * @param x Top-left X in pixels.
     * @param y Top-left Y in pixels.
     * @param w Width in pixels.
     * @param h Height in pixels.
     * @param color Color used to draw/fill the rectangle.
     * @param filled If true fills the rectangle, otherwise draws its outline.
     */
    void drawRect(int x, int y, int w, int h, const SDL_Color &color, bool filled = true);

    /**
     * @brief Convert a `Bubble::Color` enum value to an SDL_Color struct.
     * @param color The bubble color enum value.
     * @return Corresponding SDL_Color (RGBA).
     */
    SDL_Color bubbleColorToSDL(Bubble::Color color) const;

private:
    SDL_Renderer* m_renderer{nullptr};
};

#endif // SDL2_PRIMITIVES_H

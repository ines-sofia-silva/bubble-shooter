# Bubble Shooter (C++ with SDL2 Graphics)

This repository contains a C++ implementation of a Bubble Shooter style game with both CLI and SDL2 graphical interfaces.
The current version features a command-line prototype with an SDL2 graphics rendering engine.

## Current Status

- Language: C++17
- Build system: CMake (minimum 3.20)
- Graphics: SDL2 (libsdl2-dev)
- Executable target: `game`
- Platform: Linux/Windows/macOS (with C++17 compiler and SDL2 development headers)

## Build & Dependencies

Install SDL2 development headers (Linux):
```bash
sudo apt-get update && sudo apt-get install cmake libgtest-dev libsdl2-dev
```

Build from the project root:
```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/game
```

The game will:
- Open an SDL2 graphics window (500x700)
- Display the game board with hexagonal bubbles
- Show an aiming reticle that follows your mouse
- Allow CLI angle input as a fallback if preferred

## What Is Implemented

### Graphics & Rendering
- **SDL2 Rendering Engine:** Full hardware-accelerated window with graphics
- **Hexagonal Grid Visualization:** Bubbles rendered as hexagons with proper offset for hex topology
- **Color Mapping:** Visual representation of bubble colors (Red, Green, Blue, Yellow, Purple)
- **Real-time Stats Display:** Shows miss count and board information
- **Aiming Reticle:** Visual crosshair showing shooting angle based on mouse position
- **Dual Rendering:** Both SDL2 graphics and CLI text output (optional)

### Game Mechanics
- **Board Generation:** Random board with 8x8 grid of colored bubbles
- **Projectile Physics:** Realistic trajectory simulation with wall bouncing
- **Color Management:** The `ColorManager` tracks available bubble colors and their counts
- **Group Matching:** Connected groups of 3+ same-colored bubbles detected and cleared
- **Gravity System:** Unsupported bubbles fall after matches are cleared
- **Miss Tracking:** Automatic new row spawning after 3 consecutive misses
- **Win/Loss Conditions:** Game ends when bubbles reach bottom row

### Input Methods
- **Mouse-based Aiming:** Move mouse to aim, click to shoot
- **Keyboard Shortcuts:** ESC to quit (while using SDL2)
- **CLI Fallback:** Text-based angle input (20-160 degrees) still available
- **Dual Input Support:** Choose between mouse or CLI input seamlessly

### Color Mapping

- Red
- Green
- Blue
- Yellow
- Purple
- Empty/None


## Next Milestones

- Create menu and game-over screens
- Add particle effects for bubble clearing
- Implement difficulty levels and scoring system
- Implement scoring system


#pragma once

#include <iostream>

#include "SDL2/SDL.h"

class Game {
 public:
  static SDL_Renderer *sdlRenderer;
  SDL_Window *sdlWindow;

  bool isRunning;

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  Game();
  ~Game();

  void initializeWindow(const char *title, int window_xpos, int window_ypos,
                        int width, int height);

  void handleEvents();
  void update();
  void render();

  void clean();

  bool running() { return isRunning; }
};

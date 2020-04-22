#pragma once

#include <iostream>

#include "SDL2/SDL.h"
#include "player.hpp"

class Game {
 public:
  static SDL_Renderer *sdlRenderer;
  SDL_Window *sdlWindow;

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  Game();

  // Destructor is virtual as won't be using it
  virtual ~Game() = default;

  void initializeWindow(const char *title, int window_xpos, int window_ypos,
                        int width, int height);

  void handleEvents();
  void update();
  void render();

  void clean();

  bool running() { return this->_isRunning; }

 private:
  bool _isRunning;
};

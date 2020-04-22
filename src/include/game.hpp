#pragma once

#include <SDL2/SDL.h>        // SDL library.
#include <SDL2/SDL_mixer.h>  // SDL sound library.
#include <SDL2/SDL_ttf.h>    // SDL font library.

#include <iostream>

class Game {
 private:
  //  Sounds
  Mix_Chunk* _paddle_sound;
  Mix_Chunk* _wall_sound;
  Mix_Chunk* _score_sound;

  bool _isRunning;

 public:
  static SDL_Renderer* sdlRenderer;
  SDL_Window* sdlWindow;

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  Game();

  // Destructor is virtual as won't be using it
  virtual ~Game() = default;

  void initializeGameWindow(const char* title, int window_xpos, int window_ypos,
                            int width, int height);
  void initializeGame();

  void handleEvents();
  void update();
  void render();

  void clean();

  bool running() { return this->_isRunning; }
};

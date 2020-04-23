#pragma once

#include <SDL2/SDL.h>        // SDL library.
#include <SDL2/SDL_mixer.h>  // SDL sound library.
#include <SDL2/SDL_ttf.h>    // SDL font library.

#include <iostream>
#include <string>

class Game {
 private:
  //  Sounds
  Mix_Chunk* _init_sound;
  Mix_Chunk* _paddle_sound;
  Mix_Chunk* _wall_sound;
  Mix_Chunk* _score_sound;

  // In-Game Fonts
  std::string _font_location = "../assets/NES-Chimera/NES-Chimera.ttf";
  SDL_Color _font_color;
  SDL_Texture* _text_on_launch;

  // Scores.
  int _left_score;
  int _right_score;

  bool _left_score_changed;
  bool _right_score_changed;

  // Game loop
  bool _isRunning;

 public:
  static SDL_Renderer* sdlRenderer;
  SDL_Window* sdlWindow;

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  // Game realted methods
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

  // Renderer related methods
  SDL_Texture* renderText(const std::string& message, SDL_Color color,
                          int fontSize, SDL_Renderer* renderer);
};

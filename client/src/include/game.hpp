#pragma once

#include <SDL2/SDL.h>        // SDL library.
#include <SDL2/SDL_mixer.h>  // SDL sound library.
#include <SDL2/SDL_ttf.h>    // SDL font library.

#include <iostream>
#include <string>

#include "ball.hpp"
#include "network.hpp"
#include "paddle.hpp"

class Ball;
class Paddle;
class Network;

class Game {
 private:
  //  Sounds
  Mix_Chunk* _init_sound;
  Mix_Chunk* _paddle_sound;
  Mix_Chunk* _wall_sound;
  Mix_Chunk* _score_sound;
  Mix_Chunk* _pause_sound;

  // In-Game Fonts
  std::string _font_location = "../assets/NES-Chimera/NES-Chimera.ttf";
  SDL_Color _font_color;
  const int _font_size = 16;

  SDL_Texture* _text_launch;
  SDL_Texture* _text_left_score;
  SDL_Texture* _text_right_score;
  SDL_Texture* _text_paused;
  SDL_Texture* _text_winner;
  SDL_Texture* _text_restart;

  // Scores.
  int _left_score;
  int _right_score;

  bool _left_score_changed;
  bool _right_score_changed;

  // Controller
  int _mouse_x;
  int _mouse_y;

  // Game Objects
  Ball* _ball;
  Paddle* _left_paddle;
  Paddle* _right_paddle;

  // Game loop
  bool _isRunning;

  Network* _conn;

 public:
  static SDL_Renderer* sdlRenderer;
  SDL_Window* sdlWindow;

  static const int SCREEN_WIDTH = 640;
  static const int SCREEN_HEIGHT = 480;

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

  enum Status { START, INPLAY, PAUSE, COMPLETE };
  Status status;

  enum Mode { AI, MULTIP };
  Mode mode;

  // Renderer related methods
  SDL_Texture* renderText(const std::string& message, SDL_Color color,
                          int fontSize, SDL_Renderer* renderer);

  void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y,
                     SDL_Rect* clip = nullptr);

  void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst,
                     SDL_Rect* clip = nullptr);
};

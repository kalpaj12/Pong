#include "game.hpp"

SDL_Renderer* Game::sdlRenderer = nullptr;

Game::Game() { std::cout << "Game Loading" << std::endl; }

void Game::initializeGameWindow(const char* title, int window_xpos,
                                int window_ypos, int width, int height) {
  this->_isRunning = false;

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    std::cout << "SDL sub-systems initialized successfully!" << std::endl;

    sdlWindow = SDL_CreateWindow(title, window_xpos, window_ypos, width, height,
                                 SDL_WINDOW_SHOWN);
    if (sdlWindow != 0) {
      std::cout << "Window Created!" << std::endl;

      // Don't show cursor inside game window
      SDL_ShowCursor(0);

      sdlRenderer = SDL_CreateRenderer(
          sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (sdlRenderer != 0) {
        std::cout << "Renderer Created!" << std::endl;
        this->_isRunning = true;
      } else {
        std::cerr << "SDL Renderer pooped itself" << std::endl;
        std::cerr << SDL_GetError() << std::endl;

        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "SDL Window pooped itself" << std::endl;
      std::cerr << SDL_GetError() << std::endl;

      exit(EXIT_FAILURE);
    }
  } else {
    std::cerr << "SDL pooped itself" << std::endl;
    std::cerr << SDL_GetError() << std::endl;

    exit(EXIT_FAILURE);
  }
}

void Game::initializeGame() {
  // Initialise sound system
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

  this->_paddle_sound = Mix_LoadWAV("assets/audio/paddle_hit.wav");
  this->_wall_sound = Mix_LoadWAV("assets/audio/wall_hit.wav");
  this->_score_sound = Mix_LoadWAV("assets/audio/score_update.wav");
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      // User has clicked the close button
      this->_isRunning = false;
      break;

    default:
      break;
  }
}

void Game::update() {}

void Game::render() {
  SDL_RenderClear(sdlRenderer);
  SDL_RenderPresent(sdlRenderer);
}

void Game::clean() {
  SDL_DestroyWindow(sdlWindow);
  SDL_DestroyRenderer(sdlRenderer);
  SDL_Quit();

  std::cout << "Cleaned" << std::endl;
}

#include "game.hpp"

SDL_Renderer* Game::sdlRenderer;

Game::Game() {}

Game::~Game() {}

void Game::initializeWindow(const char* title, int window_xpos, int window_ypos,
                            int width, int height) {
  isRunning = false;

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    std::cout << "SDL sub-systems initialized successfully!" << std::endl;

    sdlWindow =
        SDL_CreateWindow(title, window_xpos, window_ypos, width, height, SDL_WINDOW_RESIZABLE);
    if (sdlWindow != 0) {
      std::cout << "Window Created!" << std::endl;

      sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
      if (sdlRenderer != 0) {
        std::cout << "Renderer Created!" << std::endl;
        isRunning = true;

        // Set initial Window Background to WHITE
        SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
      } else {
        std::cout << "SDL Renderer pooped itself" << std::endl;
        exit(EXIT_FAILURE);
      }
    } else {
      std::cout << "SDL Window pooped itself" << std::endl;
      exit(EXIT_FAILURE);
    }
  } else {
    std::cout << "SDL pooped itself" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      // User has clicked the close button
      isRunning = false;
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

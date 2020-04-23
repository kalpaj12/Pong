#include "game.hpp"

SDL_Renderer* Game::sdlRenderer = nullptr;

// Game Related Methods:
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

  this->_init_sound = Mix_LoadWAV("../assets/audio/init.wav");
  this->_paddle_sound = Mix_LoadWAV("../assets/audio/paddle_hit.wav");
  this->_wall_sound = Mix_LoadWAV("../assets/audio/wall_hit.wav");
  this->_score_sound = Mix_LoadWAV("../assets/audio/score_update.wav");
  std::cout << "Audio Loaded!" << std::endl;

  // Initialise Fonts
  if (TTF_Init() != -1) {
    std::cout << "TTF Loaded!" << std::endl;
    this->_font_color = {255, 255, 255, 255};
    this->_text_on_launch =
        renderText("Press SPACE to start", this->_font_color, 16, sdlRenderer);

    // Set initial scores to 0 : 0
    this->_left_score = 0;
    this->_right_score = 0;

    // Render it
    this->_left_score_changed = true;
    this->_right_score_changed = true;

    // All loaded successfully, play init sound
    // Yes, it is of SuperMario Bros.
    Mix_PlayChannel(-1, this->_init_sound, 0);

  } else {
    std::cerr << "TTF pooped itself!" << std::endl;
    std::cout << TTF_GetError() << std::endl;

    exit(EXIT_FAILURE);
  }
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
  // Free Textures
  SDL_DestroyTexture(this->_text_on_launch);

  // Free sound effects.
  Mix_FreeChunk(this->_init_sound);
  Mix_FreeChunk(this->_paddle_sound);
  Mix_FreeChunk(this->_wall_sound);
  Mix_FreeChunk(this->_score_sound);

  // Quit SDL_mixer.
  Mix_CloseAudio();

  SDL_DestroyRenderer(sdlRenderer);
  SDL_DestroyWindow(sdlWindow);

  SDL_Quit();

  std::cout << "Cleaned" << std::endl;
}

// Renderer Related Methods:
SDL_Texture* Game::renderText(const std::string& message, SDL_Color color,
                              int fontSize, SDL_Renderer* renderer) {
  TTF_Font* font = TTF_OpenFont(this->_font_location.c_str(), fontSize);

  SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);

  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return texture;
}
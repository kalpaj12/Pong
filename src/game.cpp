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

      sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
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
    this->_text_launch = renderText("Press SPACE to start", this->_font_color,
                                    this->_font_size, sdlRenderer);
    this->_text_restart =
        renderText("Press SPACE to restart", this->_font_color,
                   this->_font_size, sdlRenderer);

    // Set initial scores to 0 : 0
    this->_left_score = 0;
    this->_right_score = 0;

    // Render it
    this->_left_score_changed = true;
    this->_right_score_changed = true;

    // Create Ball
    this->_ball =
        new Ball((Game::SCREEN_WIDTH / 2) - 10, (Game::SCREEN_HEIGHT / 2) - 20);

    // Create left & right paddles
    this->_left_paddle =
        new Paddle(40, Game::SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2);

    this->_right_paddle =
        new Paddle(Game::SCREEN_WIDTH - (40 + Paddle::WIDTH),
                   Game::SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2);

    status = Game::START;

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
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        SDL_GetMouseState(&this->_mouse_x, &this->_mouse_y);
        std::cout << "Mouse Co-ordinates: (" << this->_mouse_x << ", "
                  << this->_mouse_y << ")" << std::endl;
        break;

      case SDL_QUIT:
        // User has clicked the close button
        this->_isRunning = false;
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            this->_isRunning = false;
            break;

          case SDLK_SPACE:
            if (status == Game::START) {
              Mix_HaltChannel(-1);
              status = Game::INPLAY;
            }
            break;

          default:
            break;
        }
        break;

      default:
        break;
    }
  }
}

void Game::update() {
  this->_right_paddle->set_y(this->_mouse_y);

  // @TODO: Add AI here
  this->_left_paddle->set_y(this->_mouse_y);

  if (Game::status == Game::START) {
    return;
  }

  if (Game::status == Game::INPLAY) {
    // @TODO: Ball movements
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);
  SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

  if (status == Game::START) {
    renderTexture(this->_text_launch, sdlRenderer, Game::SCREEN_WIDTH / 2 - 160,
                  Game::SCREEN_HEIGHT / 2);
  } else if (status == Game::INPLAY) {
    // Render Paddles
    SDL_Rect left_paddle = {this->_left_paddle->x_pos,
                            this->_left_paddle->y_pos, Paddle::WIDTH,
                            Paddle::HEIGHT};
    SDL_RenderFillRect(sdlRenderer, &left_paddle);

    SDL_Rect right_paddle = {this->_right_paddle->x_pos,
                             this->_right_paddle->y_pos, Paddle::WIDTH,
                             Paddle::HEIGHT};
    SDL_RenderFillRect(sdlRenderer, &right_paddle);

    // Render Ball
    SDL_Rect ball_rect = {this->_ball->x_pos, this->_ball->y_pos,
                          this->_ball->DIMENSION, this->_ball->DIMENSION};
    SDL_RenderFillRect(sdlRenderer, &ball_rect);

    // Render Score
    if (this->_left_score_changed) {
      this->_text_left_score = renderText(std::to_string(this->_left_score),
                                          this->_font_color, 24, sdlRenderer);
      this->_left_score_changed = false;
    }
    renderTexture(this->_text_left_score, sdlRenderer,
                  Game::SCREEN_WIDTH * 3 / 10 - this->_font_size,
                  Game::SCREEN_HEIGHT / 12);

    if (this->_right_score_changed) {
      this->_text_right_score = renderText(std::to_string(this->_right_score),
                                           this->_font_color, 24, sdlRenderer);
      this->_right_score_changed = false;
    }
    renderTexture(this->_text_right_score, sdlRenderer,
                  Game::SCREEN_WIDTH * 7 / 10, Game::SCREEN_HEIGHT / 12);
  }
  SDL_RenderPresent(sdlRenderer);
}

void Game::clean() {
  // Free Textures
  SDL_DestroyTexture(this->_text_launch);
  SDL_DestroyTexture(this->_text_left_score);
  SDL_DestroyTexture(this->_text_right_score);
  SDL_DestroyTexture(this->_text_winner);
  SDL_DestroyTexture(this->_text_restart);

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

void Game::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y,
                         SDL_Rect* clip) {
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  if (clip != nullptr) {
    dst.w = clip->w;
    dst.h = clip->h;
  } else {
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
  }

  renderTexture(tex, ren, dst, clip);
}

void Game::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst,
                         SDL_Rect* clip) {
  SDL_RenderCopy(ren, tex, clip, &dst);
}
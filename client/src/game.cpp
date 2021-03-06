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
  if (Game::status != Game::INPLAY) {
    // Initialise sound system
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

    this->_init_sound = Mix_LoadWAV("../assets/audio/init.wav");
    this->_paddle_sound = Mix_LoadWAV("../assets/audio/paddle_hit.wav");
    this->_wall_sound = Mix_LoadWAV("../assets/audio/wall_hit.wav");
    this->_score_sound = Mix_LoadWAV("../assets/audio/score_update.wav");
    this->_pause_sound = Mix_LoadWAV("../assets/audio/pause.wav");
    std::cout << "Audio Loaded!" << std::endl;

    // Initialise Fonts
    if (TTF_Init() != -1) {
      std::cout << "TTF Loaded!" << std::endl;
      this->_font_color = {255, 255, 255, 255};

      this->_text_launch = renderText("Press SPACE to start", this->_font_color,
                                      this->_font_size, sdlRenderer);
      this->_text_paused =
          renderText("Game Paused, press p to continue", this->_font_color,
                     this->_font_size, sdlRenderer);
      this->_text_restart =
          renderText("Press SPACE to restart", this->_font_color,
                     this->_font_size, sdlRenderer);
    } else {
      std::cerr << "TTF pooped itself!" << std::endl;
      std::cout << TTF_GetError() << std::endl;

      exit(EXIT_FAILURE);
    }

    Game::status = Game::START;

    Game::mode = Game::AI;
#ifdef MULTIPLAYER
    Game::mode = Game::MULTIP;
    std::cout << "Multiplayer enabled!" << std::endl;
    this->_conn = new Network();
    Game::status = Game::INPLAY;
#endif

    // All loaded successfully, play init sound
    // Yes, it is of SuperMario Bros.
    Mix_PlayChannel(-1, this->_init_sound, 0);
  }

  if (Game::status == Game::INPLAY) Mix_HaltChannel(-1);

  // Set initial scores to 0 : 0
  this->_left_score = 0;
  this->_right_score = 0;

  // Render it
  this->_left_score_changed = true;
  this->_right_score_changed = true;

  // Create Ball
  this->_ball =
      new Ball((Game::SCREEN_WIDTH / 2) - 10, (Game::SCREEN_HEIGHT / 2) - 20);
  this->_ball->init_ball_push();

  // Create left & right paddles
  this->_left_paddle =
      new Paddle(40, Game::SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2);

  this->_right_paddle =
      new Paddle(Game::SCREEN_WIDTH - (40 + Paddle::WIDTH),
                 Game::SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2);
}

void Game::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        if (Game::status == Game::INPLAY) {
          SDL_GetMouseState(&this->_mouse_x, &this->_mouse_y);
          // std::cout << "Mouse Co-ordinates: (" << this->_mouse_x << ", "
          //           << this->_mouse_y << ")" << std::endl;
        }
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
            if (Game::status == Game::START) {
              Mix_HaltChannel(-1);
              Game::status = Game::INPLAY;
            }

            if (Game::status == Game::COMPLETE) {
              Mix_HaltChannel(-1);
              Game::status = Game::INPLAY;
              initializeGame();
            }
            break;

          case SDLK_p: {
            bool is_valid_pause = false;
            if (Game::status == Game::INPLAY) {
              Game::status = Game::PAUSE;
              is_valid_pause = true;
            } else if (Game::status == Game::PAUSE) {
              Game::status = Game::INPLAY;
              is_valid_pause = true;
            }

            if (is_valid_pause) {
              Mix_HaltChannel(-1);
              Mix_PlayChannel(-1, this->_pause_sound, 0);
            }
          } break;

          case SDLK_f: {
            auto ToggleFullscreen = [&](SDL_Window* Window) {
              Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
              bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
              SDL_SetWindowFullscreen(Window,
                                      IsFullscreen ? 0 : FullscreenFlag);
            };
            ToggleFullscreen(sdlWindow);
            Mix_HaltChannel(-1);
            Game::status = Game::PAUSE;
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
  if (Game::status == Game::START) {
  }

  if (Game::status == Game::PAUSE) {
  }

  if (Game::status == Game::INPLAY) {
    if (Game::mode == Game::MULTIP) {
      int16_t data[4];
      data[0] = this->_conn->my_player_id;
      data[1] = this->_mouse_y;
      if (this->_conn->ball_handler) {
        data[2] =
            Game::SCREEN_WIDTH - this->_ball->x_pos - this->_ball->DIMENSION;
        data[3] = this->_ball->y_pos;
      }

      this->_conn->setrdata(data);
      this->_conn->sendServer();
      this->_conn->recServer();
    }
    // Paddles
    this->_right_paddle->set_y(this->_mouse_y);

    if (Game::mode == Game::AI) {
      //  AI trackes Ball, puff, not really AI
      this->_left_paddle->set_ai_y(this->_ball);
    }

    if (Game::mode == Game::MULTIP) {
      this->_left_paddle->set_y(this->_conn->rdata[1]);
    }

    // Update ball pos
    if (Game::mode == Game::AI || this->_conn->ball_handler) {
      this->_ball->bounced = false;
      this->_ball->x_pos += this->_ball->dx;
      this->_ball->y_pos += this->_ball->dy;
    } else {
      this->_ball->bounced = false;
      this->_ball->x_pos = this->_conn->rdata[2];
      this->_ball->y_pos = this->_conn->rdata[3];
    }
    // Ball collides to upper walls
    if (this->_ball->latitude_wall_collision()) {
      this->_ball->dy *= (-1);
      Mix_PlayChannel(-1, this->_wall_sound, 0);
    }

    // Ball collides with paddles
    if (this->_ball->paddle_collision(this->_right_paddle)) {
      Mix_PlayChannel(-1, this->_paddle_sound, 0);
      this->_ball->dx *= -1;
    } else if (this->_ball->paddle_collision(this->_left_paddle)) {
      this->_ball->dx *= -1;
      Mix_PlayChannel(-1, this->_paddle_sound, 0);
    }

    // Ball goes out from paddle side
    if (this->_ball->x_pos + this->_ball->DIMENSION >
            this->_right_paddle->x_pos &&
        !this->_ball->bounced) {
      this->_left_score++;
      this->_left_score_changed = true;
      Mix_PlayChannel(-1, this->_score_sound, 0);
      this->_ball->reset();
    } else if (this->_ball->x_pos <
                   this->_left_paddle->x_pos + this->_left_paddle->WIDTH &&
               !this->_ball->bounced) {
      this->_right_score++;
      this->_right_score_changed = true;
      Mix_PlayChannel(-1, this->_score_sound, 0);
      this->_ball->reset();
    }
  }

  if (this->_right_score == 5) {
    Game::status = Game::COMPLETE;
    this->_text_winner = renderText("Player 2 wins!", this->_font_color,
                                    this->_font_size, sdlRenderer);
    if (Game::mode == Game::MULTIP) {
      this->_conn->~Network();
      this->~Game();
    }
  }

  if (this->_left_score == 5) {
    Game::status = Game::COMPLETE;
    this->_text_winner = renderText("Player 1 wins!", this->_font_color,
                                    this->_font_size, sdlRenderer);
    if (Game::mode == Game::MULTIP) {
      this->_conn->~Network();
      this->~Game();
    }
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
    // Left and right gutter
    SDL_Rect left_line = {this->_left_paddle->x_pos - 1, 0, 1,
                          Game::SCREEN_HEIGHT};
    SDL_Rect right_line = {this->_right_paddle->x_pos + Paddle::WIDTH, 0, 1,
                           Game::SCREEN_HEIGHT};

    SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(sdlRenderer, &left_line);
    SDL_RenderFillRect(sdlRenderer, &right_line);
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

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
  } else if (Game::status == Game::PAUSE) {
    renderTexture(this->_text_paused, sdlRenderer, Game::SCREEN_WIDTH / 2 - 260,
                  Game::SCREEN_HEIGHT / 2);
  } else if (Game::status == Game::COMPLETE) {
    renderTexture(this->_text_winner, sdlRenderer, Game::SCREEN_WIDTH / 2 - 160,
                  Game::SCREEN_HEIGHT / 2 - 100);

    renderTexture(this->_text_restart, sdlRenderer,
                  Game::SCREEN_WIDTH / 2 - 180, Game::SCREEN_HEIGHT / 2);
  }
  SDL_RenderPresent(sdlRenderer);
}

void Game::clean() {
  // Free Textures
  SDL_DestroyTexture(this->_text_launch);
  SDL_DestroyTexture(this->_text_left_score);
  SDL_DestroyTexture(this->_text_right_score);
  SDL_DestroyTexture(this->_text_paused);
  SDL_DestroyTexture(this->_text_winner);
  SDL_DestroyTexture(this->_text_restart);

  // Free sound effects.
  Mix_FreeChunk(this->_init_sound);
  Mix_FreeChunk(this->_paddle_sound);
  Mix_FreeChunk(this->_wall_sound);
  Mix_FreeChunk(this->_score_sound);
  Mix_FreeChunk(this->_pause_sound);

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
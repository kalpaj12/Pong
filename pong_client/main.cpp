#include "game.hpp"

int main(void) {
  Game *game = new Game();

  game->initializeGameWindow("Pong", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, game->SCREEN_WIDTH,
                             game->SCREEN_HEIGHT);
  game->initializeGame();

  Uint32 frameBegin, runtimeFrame;
  while (game->running()) {
    frameBegin = SDL_GetTicks();

    game->handleEvents();
    game->update();
    game->render();

    runtimeFrame = SDL_GetTicks() - frameBegin;

    if (game->frameDelay > (int)runtimeFrame) {
      SDL_Delay(game->frameDelay - (int)runtimeFrame);
    }
  }

  game->clean();

  return 0;
}
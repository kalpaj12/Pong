#include "game.hpp"

int main(int argc, char const *argv[]) {
  Game *game = new Game();

  game->initializeWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         800, 640);

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
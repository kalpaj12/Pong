#include "paddle.hpp"

#include "game.hpp"

Paddle::Paddle(int pos_x, int pos_y) {
  this->x_pos = pos_x;
  this->y_pos = pos_y;
}

void Paddle::add_to_y(int new_y) {
  this->y_pos += new_y;

  if (this->y_pos <= 0)
    this->y_pos = 0;
  else if (this->y_pos + this->HEIGHT > Game::SCREEN_HEIGHT)
    this->y_pos = Game::SCREEN_HEIGHT - Paddle::HEIGHT;
}

void Paddle::set_y(int new_y) {
  if (new_y <= 0)
    new_y = 0;
  else if (new_y + this->HEIGHT > Game::SCREEN_HEIGHT)
    new_y = Game::SCREEN_HEIGHT - Paddle::HEIGHT;

  this->y_pos = new_y;
}
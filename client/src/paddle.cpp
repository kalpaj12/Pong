#include "paddle.hpp"

#include "game.hpp"

Paddle::Paddle(int pos_x, int pos_y) {
  this->x_pos = pos_x;
  this->y_pos = pos_y;
}

void Paddle::set_y(int new_y) {
  if (new_y <= 0)
    new_y = 0;
  else if (new_y + this->HEIGHT > Game::SCREEN_HEIGHT)
    new_y = Game::SCREEN_HEIGHT - Paddle::HEIGHT;

  this->y_pos = new_y;
}

// AI for left paddle(player 1)
void Paddle::set_ai_y(Ball *ball) {
  // @TODO: Sometimes mess this code, so that AI looses :)
  int curr_ball_pos = ball->y_pos;

  this->y_pos =
      curr_ball_pos + (int)floor((Ball::DIMENSION - Paddle::HEIGHT) / 2);
  if (this->y_pos < 0) this->y_pos = 0;
  if (this->y_pos + Paddle::HEIGHT > Game::SCREEN_HEIGHT)
    this->y_pos = Game::SCREEN_HEIGHT - Paddle::HEIGHT;
}
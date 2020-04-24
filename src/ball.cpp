#include "ball.hpp"

#include "game.hpp"

Ball::Ball(int pos_x, int pos_y) {
  this->x_pos = pos_x;
  this->y_pos = pos_y;

  dx = 0;
  dy = 0;

  y_predicted = 0;
  bounced = false;
  speed = 8;
  angle = 0;
  hits = 0;
}

bool Ball::latitude_wall_collision() {
  if (this->y_pos + this->DIMENSION >= Game::SCREEN_HEIGHT || this->y_pos <= 0)
    return true;
  return false;
}

void Ball::update_speed() {
  if (hits == 3) {
    speed += 2;
    hits = 0;
  }
}

void Ball::reset() {
  x_pos = Game::SCREEN_WIDTH / 2;
  y_pos = Game::SCREEN_HEIGHT / 2;

  dx = 0;
  dy = 0;

  bounced = false;
  speed = 8;
  hits = 0;
}
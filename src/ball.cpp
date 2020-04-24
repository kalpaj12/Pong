#include "ball.hpp"

#include <random>

#include "game.hpp"

Ball::Ball(int pos_x, int pos_y) {
  this->x_pos = pos_x;
  this->y_pos = pos_y;

  dx = 0;
  dy = 0;

  y_predicted = 0;
  angle = 0;
}

bool Ball::latitude_wall_collision() {
  if (this->y_pos + this->DIMENSION >= Game::SCREEN_HEIGHT || this->y_pos <= 0)
    return true;
  return false;
}

bool Ball::paddle_collision(Paddle *paddle) {
  if (paddle->x_pos > Game::SCREEN_WIDTH / 2) {
    if (this->x_pos + this->dx >= paddle->x_pos &&
        this->y_pos + this->dy >= paddle->y_pos &&
        this->y_pos + this->dy <= paddle->y_pos + Paddle::HEIGHT)
      return true;
    else
      return false;
  } else {
    if (this->x_pos + this->dx <= paddle->x_pos &&
        this->y_pos + this->dy >= paddle->y_pos &&
        this->y_pos + this->dy <= paddle->y_pos + Paddle::HEIGHT)
      return true;
    else
      return false;
  }
  return false;
}

void Ball::init_ball_push() {
  // https://stackoverflow.com/questions/5743678/generate-random-number-between-0-and-10
  std::random_device rdev;
  std::mt19937 rgen(rdev());
  std::uniform_int_distribution<int> idist(0, 1);

  int launch_direction = 1 + (-2) * (idist(rgen) % 2);  // -1 or 1

  std::uniform_int_distribution<int> iangle(-60, 60);
  this->angle = iangle(rgen);  // -60 to 60 degrees

  this->dx = launch_direction * 8 * std::cos(this->angle * M_PI / 180.0);
  this->dy = launch_direction * 8 * std::sin(this->angle * M_PI / 180.0);
}

void Ball::reset() {
  x_pos = Game::SCREEN_WIDTH / 2;
  y_pos = Game::SCREEN_HEIGHT / 2;

  init_ball_push();
}
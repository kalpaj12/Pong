#include "ball.hpp"

#include <random>

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

void Ball::init_ball_push() {
  // https://stackoverflow.com/questions/5743678/generate-random-number-between-0-and-10
  std::random_device rdev;
  std::mt19937 rgen(rdev());
  std::uniform_int_distribution<int> idist(0, 1);

  int launch_direction = 1 + (-2) * (idist(rgen) % 2);  // -1 or 1

  std::uniform_int_distribution<int> iangle(-60, 60);
  this->angle = iangle(rgen);  // -30 to 30 degrees

  this->dx = launch_direction * speed * std::cos(this->angle * M_PI / 180.0);
  this->dy = launch_direction * speed * std::sin(this->angle * M_PI / 180.0);
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

  init_ball_push();

  bounced = false;
  speed = 8;
  hits = 0;
}
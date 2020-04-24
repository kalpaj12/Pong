#pragma once

#include "paddle.hpp"

class Paddle;
class Ball {
 public:
  static const int DIMENSION = 10;

  Ball(int pos_x, int pos_y);
  virtual ~Ball() = default;

  int x_pos;
  int y_pos;
  int y_predicted;

  int dx;
  int dy;

  double angle;

  void init_ball_push();
  void update_speed();

  bool latitude_wall_collision();
  bool paddle_collision(Paddle *paddle);
  void reset();
};
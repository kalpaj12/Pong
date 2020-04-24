#pragma once

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

  int speed;
  int hits;  // to increase speed of the ball

  double angle;

  bool bounced;

  bool latitude_wall_collision();
  void update_speed();
  void reset();
};
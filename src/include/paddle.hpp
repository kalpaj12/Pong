#pragma once

class Paddle {
 public:
  static const int HEIGHT = 60;
  static const int WIDTH = 10;

  int x_pos;
  int y_pos;

  Paddle(int pos_x, int pos_y);
  virtual ~Paddle() = default;

  // As paddle moves only in y-direction
  void add_to_y(int new_y);
  void set_y(int new_y);
};
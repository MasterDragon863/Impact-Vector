#pragma once

/**
 * @class Input
 * @brief Simple Struct that handles input
 * @size 4 bytes
 */
struct Input {
  bool right;
  bool left;
  bool jump;
  bool dash;

  void addInput(bool right, bool left, bool jump, bool dash) {
    this->right = right;
    this->left = left;
    this->jump = jump;
    this->dash = dash;
  }
};

#pragma once

/**
 * @class Physics
 * @brief Struct that handles the data for simple phyics
 * @size 16 bytes
 */
struct Physics {
  float gravity;
  float friction;
  float mass;
  float bounciness;

  Physics(float gravity = 9.8f, float friction = 0.0f, float mass = 1.0f,
          float bounciness = 0.0f)
      : gravity(gravity),
        friction(friction),
        mass(mass),
        bounciness(bounciness) {}
};

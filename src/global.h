#pragma once

#include "config.h"

#include <SFML/Graphics.hpp>  // for sf::Vector2i
#include <cstdlib>

struct CompareTilesPositions {
  bool operator()(const sf::Vector2<int> &a, const sf::Vector2<int> &b) const {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
  }
};

// Multiplying a vector by a scalar
static sf::Vector2i operator*(int scalar, const sf::Vector2i &vector) {
  sf::Vector2i toReturn = vector;
  int sign = (scalar < 0) ? -1 : 1;
  for (int i = 0; i < scalar; i += 1) toReturn += sign * vector;
  return toReturn;
}

// Vector of world positions
typedef std::vector<sf::Vector2i> PosVec;
#pragma once

#include "config.h"

#include <math.h>
#include <SFML/Graphics.hpp>

struct CompareTilesPositions {
  bool operator()(const sf::Vector2<int> &a, const sf::Vector2<int> &b) const {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
  }
};

struct OrderedPosCouple {
  static CompareTilesPositions cmp;

  OrderedPosCouple(const sf::Vector2i &pos_a, const sf::Vector2i &pos_b) {
    if (cmp(pos_a, pos_b)) {
      first = pos_a;
      second = pos_b;
    } else {
      first = pos_b;
      second = pos_a;
    }
  }

  sf::Vector2i get_vector() const {
    return {second.x - first.x, second.y - first.y};
  }

  sf::Vector2i first, second;
};

struct CompareOrderedPosCouple {
  bool operator()(const OrderedPosCouple &a, const OrderedPosCouple &b) const {
    return OrderedPosCouple::cmp(a.first, b.first);
  }
};

// Multiplying a vector by a scalar
template <typename T, typename U>
static sf::Vector2<T> operator*(U scalar, const sf::Vector2<T> &vector) {
  sf::Vector2<T> toReturn = vector;
  return {toReturn.x * scalar, toReturn.y * scalar};
}

// Multiplying two vectors component by component
template <typename T>
static sf::Vector2<T> operator*(const sf::Vector2<T> &a,
                                const sf::Vector2<T> &b) {
  return {a.x * b.x, a.y * b.y};
}

// Vector of world positions
typedef std::vector<sf::Vector2i> PosVec;

inline float Euclidean_norm(const sf::Vector2f &vect) {
  return sqrt(vect.x * vect.x + vect.y * vect.y);
}

inline sf::Vector2f get_normal_unit_vector(const sf::Vector2f &vect) {
  return {vect.y / Euclidean_norm(vect), -1 * vect.x / Euclidean_norm(vect)};
}
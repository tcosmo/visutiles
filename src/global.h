#pragma once

#include "config.h"

#include <assert.h>
#include <SFML/Graphics.hpp>  // for sf::Vector2i
#include <cstdlib>

#ifdef _WIN32
#define OS_FILE_SEPARATOR '\\'
#else
#define OS_FILE_SEPARATOR '/'
#endif

#define DEBUG 1

#include <string.h>

#define __FILENAME__ \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define debug_print(fmt, ...)                                               \
  do {                                                                      \
    if (DEBUG)                                                              \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILENAME__, __LINE__, __func__, \
              __VA_ARGS__);                                                 \
  } while (0)

#define error_log(fmt, ...)                                               \
  do {                                                                    \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILENAME__, __LINE__, __func__, \
            __VA_ARGS__);                                                 \
  } while (0)

#define fatal_error_log(fmt, ...)                                         \
  do {                                                                    \
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILENAME__, __LINE__, __func__, \
            __VA_ARGS__);                                                 \
    exit(1);                                                              \
  } while (0)

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
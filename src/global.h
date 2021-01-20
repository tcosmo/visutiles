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

#define warning_log(fmt, ...)                                           \
  do {                                                                  \
    fprintf(stdout, "WARNING %s:%d:%s(): " fmt, __FILENAME__, __LINE__, \
            __func__, __VA_ARGS__);                                     \
  } while (0)

#define error_log(fmt, ...)                                           \
  do {                                                                \
    fprintf(stderr, "ERROR %s:%d:%s(): " fmt, __FILENAME__, __LINE__, \
            __func__, __VA_ARGS__);                                   \
  } while (0)

#define fatal_error_log(fmt, ...)                                     \
  do {                                                                \
    fprintf(stderr, "FATAL %s:%d:%s(): " fmt, __FILENAME__, __LINE__, \
            __func__, __VA_ARGS__);                                   \
    exit(1);                                                          \
  } while (0)

struct CompareTilesPositions {
  bool operator()(const sf::Vector2<int> &a, const sf::Vector2<int> &b) const {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
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
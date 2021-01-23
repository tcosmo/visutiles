#pragma once

#include "config.h"
#include "geometry.h"

#include <assert.h>
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

// Some graphic parameters
const static int GRAPHIC_EDGE_THICK = 5;
const static int GRAPHIC_EDGE_WIDTH = 64;
const static int GRAPHIC_TILE_SIZE = GRAPHIC_EDGE_WIDTH;

// A bit of
// https://github.com/chrisBRN/Lots_Of_Colors/blob/master/lots_of_colors_sfml.h

static const sf::Color cga_02{245, 85, 85};    // FF555555
static const sf::Color cga_03{250, 170, 170};  // FFAAAAAA
static const sf::Color cga_05{240, 0, 10};     // FF0000AA
static const sf::Color cga_06{245, 85, 95};    // FF5555FF
static const sf::Color cga_07{240, 10, 160};   // FF00AA00
static const sf::Color cga_08{245, 95, 245};   // FF55FF55
static const sf::Color cga_09{240, 10, 170};   // FF00AAAA
static const sf::Color cga_10{245, 95, 255};   // FF55FFFF
static const sf::Color cga_11{250, 160, 0};    // FFAA0000
static const sf::Color cga_12{255, 245, 85};   // FFFF5555
static const sf::Color cga_13{250, 160, 10};   // FFAA00AA
static const sf::Color cga_14{255, 245, 95};   // FFFF55FF
static const sf::Color cga_15{250, 165, 80};   // FFAA5500
static const sf::Color cga_16{255, 255, 245};  // FFFFFF55

static const std::vector<sf::Color> COLOR_WHEEL = {
    sf::Color::Green, sf::Color::Red, cga_02, cga_03, cga_05, cga_06,
    cga_07,           cga_08,         cga_09, cga_10, cga_11, cga_12,
    cga_13,           cga_14,         cga_15, cga_16};
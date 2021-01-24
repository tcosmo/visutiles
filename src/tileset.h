#pragma once

#include <map>
#include <string>
#include <vector>

#include "third_party/json11/json11.hpp"

#include "global.h"
#include "square_grid.h"

typedef std::string EdgeAlphabetName;
// We use char for the following in order to draw information on the screen
// easily, relying on ASCII textures
typedef char EdgeChar;
typedef char TileName;

// An edge color is given by an alphabet name and a character in that alphabet
typedef std::pair<std::string, int> EdgeColor;
static const std::pair<std::string, int> ANY_EDGE_COLOR = {"", -1};

class Tileset {
 public:
  Tileset(std::string json_file_dir, std::string json_filename);
  ~Tileset();

  // Return the set of tiles satisfying some given edge colors constraints
  std::vector<TileName> Wang_query(std::array<EdgeColor, 4> edge_constraints);

  void print_edge_color(const EdgeColor& c) {
    printf("%s, `%c`\n", c.first.c_str(), alphabet[c.first][c.second]);
  }

  const std::vector<EdgeAlphabetName>& get_alphabet_names() const {
    return alphabet_names;
  }

  char get_edge_char(EdgeColor c) const {
    assert(!(c.second < 0 || c.second >= alphabet.at(c.first).size()));
    return alphabet.at(c.first).at(c.second);
  }

  const std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>& get_tile_spec(
      TileName tile_name) const {
    return tile_specification.at(tile_name);
  }

  std::vector<EdgeColor> all_edge_colors;

 private:
  std::string json_file_dir;
  std::string json_filename;

  std::map<std::array<EdgeColor, 4>, std::vector<TileName>> memoize_queries;

  void parse_json_file();

  std::vector<EdgeAlphabetName> alphabet_names;
  std::map<EdgeAlphabetName, std::vector<EdgeChar>> alphabet;
  std::array<EdgeAlphabetName, SQUARE_GRID_NEIGHBORING_SIZE> alphabet_on_edge;
  std::map<TileName, std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>>
      tile_specification;

  json11::Json json_doc;
};
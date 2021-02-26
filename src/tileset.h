#pragma once

#include <algorithm>
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
static const char* MISMATCH_COLOR_NAME = "mismatch";
static const std::pair<std::string, int> MISMATCH_EDGE_COLOR = {
    MISMATCH_COLOR_NAME, -2};

struct CompareEdgeColors {
  bool operator()(const EdgeColor& a, const EdgeColor& b) const {
    if (a.first != b.first)
      return std::lexicographical_compare(a.first.begin(), a.first.end(),
                                          b.first.begin(), b.first.end());
    return a.second < b.second;
  }
};

typedef std::set<EdgeColor, CompareEdgeColors> ColorSet;

typedef std::pair<std::array<ColorSet, SQUARE_GRID_NEIGHBORING_SIZE>,
                  std::vector<TileName>>
    PossibleEdgesColorsAndTilesNames;

class Tileset {
 public:
  Tileset() {}
  Tileset(std::string json_file_dir, std::string json_filename);
  Tileset(std::string json_file_path);
  ~Tileset();

  std::string get_json_file_path() { return json_file_path; }

  // Return the set of possible edge colors and corresponding tile names
  // given a set of edge constraints
  PossibleEdgesColorsAndTilesNames Wang_query(
      std::array<EdgeColor, 4> edge_constraints);

  void print_edge_color(const EdgeColor& c) const {
    printf("%s, `%c`\n", c.first.c_str(), alphabet.at(c.first).at(c.second));
  }

  const std::vector<EdgeAlphabetName>& get_alphabet_names() const {
    return alphabet_names;
  }

  bool is_valid_edge_color(EdgeColor c) const {
    return alphabet.find(c.first) != alphabet.end() && c.second >= 0 &&
           c.second < alphabet.at(c.first).size();
  }

  char get_edge_char(EdgeColor c) const {
    assert(is_valid_edge_color(c));

    return alphabet.at(c.first).at(c.second);
  }

  const std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>& get_tile_spec(
      TileName tile_name) const {
    return tile_specification.at(tile_name);
  }

  std::vector<EdgeColor> all_edge_colors;

 private:
  std::string json_file_path;

  std::map<std::array<EdgeColor, 4>, PossibleEdgesColorsAndTilesNames>
      memoize_queries;

  void parse_json_file();

  std::vector<EdgeAlphabetName> alphabet_names;
  std::map<EdgeAlphabetName, std::vector<EdgeChar>> alphabet;
  std::array<EdgeAlphabetName, SQUARE_GRID_NEIGHBORING_SIZE> alphabet_on_edge;
  std::map<TileName, std::array<EdgeColor, SQUARE_GRID_NEIGHBORING_SIZE>>
      tile_specification;

  json11::Json json_doc;
};
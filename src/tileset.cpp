#include "tileset.h"

#include <fstream>
#include <string>

Tileset::Tileset(std::string json_file_dir, std::string json_filename) {
  json_file_path = json_file_dir + OS_FILE_SEPARATOR + json_filename;
}

Tileset::Tileset(std::string json_file_path) : json_file_path(json_file_path) {
  parse_json_file();
}

bool edge_constraints_satisfied_by_tile(
    const std::array<EdgeColor, 4>& constraint,
    const std::array<EdgeColor, 4>& tile_spec) {
  for (size_t i_dir = 0; i_dir < SQUARE_GRID_NEIGHBORING_SIZE; i_dir += 1) {
    if (constraint[i_dir] == ANY_EDGE_COLOR) continue;
    if (constraint[i_dir] != tile_spec[i_dir]) return false;
  }
  return true;
}

PossibleEdgesColorsAndTilesNames Tileset::Wang_query(
    std::array<EdgeColor, 4> edge_constraints) {
  if (memoize_queries.find(edge_constraints) != memoize_queries.end()) {
    return memoize_queries[edge_constraints];
  }

  std::array<ColorSet, SQUARE_GRID_NEIGHBORING_SIZE> edges_color_to_return;
  std::vector<TileName> tiles_to_return;

  for (const std::pair<TileName, std::array<EdgeColor, 4>>& name_and_spec :
       tile_specification) {
    if (edge_constraints_satisfied_by_tile(edge_constraints,
                                           name_and_spec.second)) {
      tiles_to_return.push_back(name_and_spec.first);
      for (size_t i_dir = 0; i_dir < SQUARE_GRID_NEIGHBORING_SIZE; i_dir += 1)
        edges_color_to_return[i_dir].insert(name_and_spec.second[i_dir]);
    }
  }

  PossibleEdgesColorsAndTilesNames to_return =
      std::make_pair(edges_color_to_return, tiles_to_return);
  memoize_queries[edge_constraints] = to_return;
  return to_return;
}

char string_to_char(const std::string& s) {
  if (s.size() == 0) {
    fatal_error_log("Empty letter in `%s`. Abort.\n", s.c_str());
  }

  if (s.size() > 1) {
    warning_log("Ignoring multi-char letter `%s`, taking only first char.\n",
                s.c_str());
  }

  return s.at(0);
}

void Tileset::parse_json_file() {
  // TODO: add error handling for missing mandatory keys: "alphabets",
  // "alphabet_on_edge", "tiles"
  std::ifstream ifs(json_file_path);
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));

  std::string err;
  json_doc = json_doc.parse(content, err);

  json11::Json::object json_dict = json_doc.object_items();
  for (const std::pair<std::string, json11::Json>& key_value :
       json_dict["alphabets"].object_items()) {
    if (std::find(alphabet_names.begin(), alphabet_names.end(),
                  key_value.first) != alphabet_names.end()) {
      fatal_error_log("Two alphabets with name `%s`. Abort.\n",
                      key_value.first.c_str());
    }

    alphabet_names.push_back(key_value.first);

    json11::Json::array alphabet_json = key_value.second.array_items();

    size_t i_letter = 0;
    for (const json11::Json& letter : alphabet_json) {
      EdgeChar char_letter = string_to_char(letter.string_value());

      if (std::find(alphabet[key_value.first].begin(),
                    alphabet[key_value.first].end(),
                    char_letter) != alphabet[key_value.first].end()) {
        fatal_error_log("Alphabet `%s` contains letter `%c` twice. Abort.\n",
                        key_value.first.c_str(), char_letter);
      }

      alphabet[key_value.first].push_back(char_letter);

      all_edge_colors.push_back(std::make_pair(key_value.first, i_letter));
      i_letter += 1;
    }
  }

  json11::Json::array alphabet_on_edge_json =
      json_dict["alphabet_on_edge"].array_items();

  if (alphabet_on_edge.size() != SQUARE_GRID_NEIGHBORING_SIZE) {
    fatal_error_log(
        "Tiles have four edges. Not the case in `alphabet_on_edge` in %s. "
        "Abort.\n",
        json_file_path.c_str());
  }

  size_t i_edge = 0;
  for (const json11::Json& alphabet_name : alphabet_on_edge_json) {
    alphabet_on_edge[i_edge] = alphabet_name.string_value();
    i_edge += 1;
  }

  json11::Json::object tiles_json = json_dict["tiles"].object_items();
  for (const std::pair<std::string, json11::Json>& key_value : tiles_json) {
    TileName tile_name = string_to_char(key_value.first);

    json11::Json::array edge_array = tiles_json[key_value.first].array_items();
    if (edge_array.size() != SQUARE_GRID_NEIGHBORING_SIZE) {
      fatal_error_log(
          "Tiles have four edges. Not the case in `tiles[%s]` in %s. "
          "Abort.\n",
          key_value.first.c_str(), json_file_path.c_str());
    }

    size_t i_edge = 0;
    for (const json11::Json& edge_letter_index_json : edge_array) {
      int letter_index = (edge_letter_index_json.int_value());

      assert(letter_index >= 0 &&
             letter_index < alphabet[alphabet_on_edge[i_edge]].size());
      tile_specification[tile_name][i_edge] =
          std::make_pair(alphabet_on_edge[i_edge], (size_t)letter_index);

      i_edge += 1;
    }
  }
}

Tileset::~Tileset() {}
#include "input_factory.h"
#include "Collatz.h"

void InputFactory::build_initial_configuration() {
  std::string err;
  json_doc = json_doc.parse(input_json, err);

  json11::Json::object json_dict = json_doc.object_items();
  std::string tileset_path = json_dict["tileset"].string_value();

  tileset = Tileset(tileset_path);

  json11::Json::object input_dict = json_dict["input"].object_items();
  json11::Json::array edges = input_dict["edges"].array_items();

  for (const json11::Json& edge_desc : edges) {
    json11::Json::array edge_pos_and_color = edge_desc.array_items();
    json11::Json::array edge_pos = edge_pos_and_color[0].array_items();

    std::array<sf::Vector2i, 2> vec_edge_pos;
    size_t i = 0;
    for (json11::Json pos : edge_pos) {
      json11::Json coords = pos.array_items();
      vec_edge_pos[i] = {coords[0].int_value(), coords[1].int_value()};
      i += 1;
    }

    json11::Json::array edge_color = edge_pos_and_color[1].array_items();
    std::string alphabet_name = edge_color[0].string_value();
    size_t symbol_index = edge_color[1].int_value();

    OrderedPosCouple pos_couple(vec_edge_pos);

    EdgeColor the_edge_color = std::make_pair(alphabet_name, symbol_index);

    initial_configuration[pos_couple] = the_edge_color;
    if (!tileset.is_valid_edge_color(the_edge_color)) {
      fatal_error_log(
          "Edge color `(%s,%d)` is not valid for tileset `%s`. Abort.",
          the_edge_color.first.c_str(), the_edge_color.second,
          tileset_path.c_str());
    }
  }
}

EdgeMap InputFactory::get_initial_configuration() {
  return initial_configuration;
}
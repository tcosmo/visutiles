#include <cstdio>

#include "graphic_engine.h"

#include "tileset.h"
#include "world.h"
#include "world_view.h"

#include "arguments.h"
#include "input_factory.h"

#include <iostream>

int main(int argc, char** argv) {
  // Command line arguments
  Arguments arguments;
  parseArguments(argc, argv, arguments);

  // Read input json on std::cin
  std::string input_json = "";
  if (arguments.inputType == STDIN) {
    std::string line;
    while (std::getline(std::cin, line)) {
      input_json += line;
    }
  }

  InputFactory input_factory(input_json);
  Tileset tileset = input_factory.tileset;

  World w(tileset);
  EdgeMap edges = input_factory.get_initial_configuration();
  w.set_input_edges(edges);

  if (arguments.partial_dump)
    w.set_edges_pos_to_check(input_factory.get_edges_to_check());

  // World view
  // TODO: move this font loader somewhere else
  sf::Font default_font;
  if (!default_font.loadFromFile(DEFAULT_FONT))
    fatal_error_log("Could not load font `%s`! Abort.\n", DEFAULT_FONT);
  WorldView w_view(tileset, w, default_font);
  if (!arguments.no_gui) w_view.update();

  for (size_t i = 0; i < arguments.run_n_steps; i += 1) {
    w.update();
    if (!arguments.no_gui) w_view.update();
  }

  // Gui mode
  if (!arguments.no_gui) {
    // Graphic engine
    GraphicEngine engine(w, w_view, tileset, 1350, 900);
    engine.run();
  } else {
    if (!arguments.partial_dump) {
      printf("%s\n", w.json_dumps().c_str());
    } else
      printf("%s\n", w.json_dumps_check().c_str());
  }

  return 0;
}
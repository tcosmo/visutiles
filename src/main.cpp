#include <cstdio>

#include "graphic_engine.h"

#include "tileset.h"
#include "world.h"
#include "world_controller.h"
#include "world_view.h"

#include "arguments.h"
#include "input_factory.h"

void input_world(const Arguments& arguments, Tileset& tileset,
                 WorldController& w_controller) {
  std::unique_ptr<InputFactory> input_factory;

  input_factory = std::unique_ptr<InputFactory>(new CollatzInputFactory(
      arguments.inputType, arguments.inputStr, tileset));

  w_controller.init_world(
      std::move(input_factory->get_initial_configuration()));
}

int main(int argc, char** argv) {
  // Command line arguments
  Arguments arguments;
  parseArguments(argc, argv, arguments);

  // Tileset
  Tileset Collatz_tileset("assets/tilesets/CollatzTileset",
                          "CollatzTileset.json");

  World w(Collatz_tileset);
  WorldController w_controller(w, Collatz_tileset);

  // Build initial configuration from parsed command line args
  input_world(arguments, Collatz_tileset, w_controller);

  sf::Font default_font;

  if (!default_font.loadFromFile(DEFAULT_FONT)) {
    fatal_error_log("Could not load font `%s`! Abort.\n", DEFAULT_FONT);
  }

  // World view
  WorldView w_view(Collatz_tileset, w_controller, default_font);
  w_view.update();

  // Simulation engine
  GraphicEngine engine(w_controller, w_view, Collatz_tileset, 1200, 800);
  engine.run();

  return 0;
}
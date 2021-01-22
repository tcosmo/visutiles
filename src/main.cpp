#include <cstdio>

#include "graphic_engine.h"

#include "world.h"
#include "world_controller.h"
#include "world_view.h"

#include "arguments.h"
#include "input_factory.h"

int main(int argc, char** argv) {
  // Command line arguments
  Arguments arguments;
  parseArguments(argc, argv, arguments);

  // Tileset
  Tileset CollatzTileset("assets/tilesets/CollatzTileset",
                         "CollatzTileset.tsx");

  // World & controller
  World w(CollatzTileset);
  WorldController w_controller(w);

  // Build initial configuration from parsed command line args
  std::unique_ptr<InputFactory> input_factory;

  if (arguments.inputType == COLLATZ_PARITY_VECTOR) {
    input_factory = std::unique_ptr<InputFactory>(new CollatzInputFactory(
        arguments.inputType, arguments.inputStr, CollatzTileset));
  } else {
    input_factory = std::unique_ptr<InputFactory>(
        new InputFactory(arguments.inputType, arguments.inputStr));
  }

  w_controller.init_world(
      std::move(input_factory->get_initial_configuration()));

  // World view
  WorldView w_view(CollatzTileset, w_controller);
  w_view.update();

  // Simulation engine
  GraphicEngine engine(w_controller, w_view, CollatzTileset, 1200, 800);
  engine.run();

  return 0;
}
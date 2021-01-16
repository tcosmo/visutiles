#include <cstdio>

#include "graphic_engine.h"

#include "world.h"
#include "world_controller.h"
#include "world_view.h"

int main() {
  Tileset CollatzTileset("assets/tilesets/CollatzTileset",
                         "CollatzTileset.tsx");

  World w(CollatzTileset);

  /*WorldController w_controller(w);
  WorldView w_view(w_controller);

  GraphicEngine engine(w_controller, w_view, 1200, 800);

  engine.run();*/
}
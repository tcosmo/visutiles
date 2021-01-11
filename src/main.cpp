#include <cstdio>

#include "graphic_engine.h"
#include "world.h"

int main() {
  World w;
  GraphicEngine engine(w, 1200, 800);

  engine.run();
}
// The goal of input factories is to transform command line
// input stings into initial configurations for tile assembly systems
#pragma once

#include "config.h"
#include "global.h"

#include "arguments.h"
#include "world.h"

class InputFactory {
 public:
  InputFactory(InputType input_type, const std::string& input_str)
      : input_type(input_type), input_str(input_str) {}

  virtual ~InputFactory() {}

  virtual TileMap get_initial_configuration();

 protected:
  InputType input_type;
  std::string input_str;
};

class CollatzInputFactory : public InputFactory {
 public:
  CollatzInputFactory(InputType input_type, const std::string& input_str,
                      Tileset& CollatzTileset)
      : InputFactory(input_type, input_str), CollatzTileset(CollatzTileset) {}

  ~CollatzInputFactory() {}

  TileMap get_initial_configuration();
  TileMap build_parity_vector_initial_configuration();

 private:
  Tileset& CollatzTileset;  // not const because tileset query cannot be made
                            // const because of memoization
};
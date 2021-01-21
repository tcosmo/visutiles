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

 private:
  InputType input_type;
  std::string input_str;
};

class CollatzInputFactory : public InputFactory {
 public:
  CollatzInputFactory(InputType input_type, const std::string& input_str)
      : InputFactory(input_type, input_str) {}

  ~CollatzInputFactory() {}

  TileMap get_initial_configuration();
};
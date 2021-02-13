// The goal of input factories is to transform command line
// input stings into initial configurations for tile assembly systems
#pragma once

#include "config.h"
#include "global.h"

#include "arguments.h"
#include "world.h"

#include "third_party/json11/json11.hpp"

#include "tileset.h"

class InputFactory {
 public:
  InputFactory(std::string input_json) : input_json(input_json) {
    build_initial_configuration();
  }

  ~InputFactory() {}

  EdgeMap get_initial_configuration();

  Tileset tileset;

 protected:
  std::string input_json;

 private:
  void build_initial_configuration();
  EdgeMap initial_configuration;
  json11::Json json_doc;
};
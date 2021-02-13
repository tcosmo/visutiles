#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include "config.h"

enum InputType { NONE = 0, STDIN };

// https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
class InputParser {
 public:
  InputParser(int &argc, char **argv) {
    for (int i = 1; i < argc; ++i) this->tokens.push_back(std::string(argv[i]));
  }
  /// @author iain
  const std::string &getCmdOption(const std::string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
      return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
  }
  /// @author iain
  bool cmdOptionExists(const std::string &option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option) !=
           this->tokens.end();
  }

 private:
  std::vector<std::string> tokens;
};

struct InputOption {
  const char *longOption;
  char shortOption;
  const char *argumentHelper;
  const char *helpString;
};

static const size_t DEFAULT_OPTION_COUNT = 3;
static const size_t CUSTOM_OPTION_COUNT = 1;
static const size_t OPTION_COUNT = DEFAULT_OPTION_COUNT + CUSTOM_OPTION_COUNT;

static InputOption options[OPTION_COUNT] = {
    // Default options
    {"help", 'h', NULL, "Give this help list"},
    {"usage", 'u', NULL, "Give a short usage message"},
    {"version", 'V', NULL, "Print program version"},

    // Custom options
    {"stdin input", 'i', NULL, "Reads input (see `input_spec.md`) on stdin"},
};

static const char doc[] =
    "Welcome to VisuTiles: a Wang tile "
    "simulator.\nhttps://github.com/tcosmo/visutiles";
static const char *program_bug_address = "tristan.sterin@mu.ie";
static const char *program_version = VERSION;

struct Arguments {
  InputType inputType;
  std::string inputStr;

  Arguments() : inputType(NONE) {}
};

void parseArguments(int argc, char *argv[], Arguments &arguments);
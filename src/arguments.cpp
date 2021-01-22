#include "arguments.h"

void usagePage() {
  printf("Usage: ./%s ", visutiles_PROG_NAME_EXEC);
  int iOption = 0;
  while (iOption < OPTION_COUNT) {
    printf(" [-%c", options[iOption].shortOption);
    if (options[iOption].argumentHelper)
      printf(" %s", options[iOption].argumentHelper);
    printf("] ");
    iOption += 1;
  }
  iOption = 0;
  while (iOption < OPTION_COUNT) {
    printf(" [--%s", options[iOption].longOption);
    if (options[iOption].argumentHelper)
      printf(" %s", options[iOption].argumentHelper);
    printf("] ");
    iOption += 1;
  }
  printf("\n");
}

bool option_exists(const InputParser &input, const InputOption &opt) {
  std::string short_opt = "-";
  short_opt.push_back(opt.shortOption);

  return input.cmdOptionExists(short_opt) ||
         input.cmdOptionExists("--" + std::string(opt.longOption));
}

std::string str_or(const std::string a, const std::string b) {
  if (a.size() == 0) return b;
  return a;
}

std::string get_cmd_option(const InputParser &input, const InputOption &opt) {
  std::string short_opt = "-";
  short_opt.push_back(opt.shortOption);
  return str_or(input.getCmdOption("--" + std::string(opt.longOption)),
                input.getCmdOption(short_opt));
}

void helpPage() {
  printf("Usage ./%s [OPTION...]\n", visutiles_PROG_NAME_EXEC);
  printf("%s\n\n", doc);
  int iOption = 0;
  while (iOption < OPTION_COUNT) {
    printf("   -%c,  --%s", options[iOption].shortOption,
           options[iOption].longOption);
    if (options[iOption].argumentHelper)
      printf(" %s", options[iOption].argumentHelper);
    printf("\n");
    printf("\t\t\t%s\n", options[iOption].helpString);
    iOption += 1;
  }
  printf("\n");
  printf("Report bugs to: %s\n", program_bug_address);
  printf("\n");
}

void exec_default_option(const InputOption &opt) {
  switch (opt.shortOption) {
    case 'h':
      helpPage();
      break;

    case 'u':
      usagePage();
      break;

    case 'V':
      printf("Version %s\n", program_version);
      break;

    default:
      break;
  }
}

void parseArguments(int argc, char *argv[], Arguments &arguments) {
  InputParser input(argc, argv);

  for (size_t i_option = 0; i_option < DEFAULT_OPTION_COUNT; i_option += 1) {
    const InputOption &opt = options[i_option];
    if (option_exists(input, opt)) {
      exec_default_option(opt);
      exit(0);
    }
  }

  // Collatz parity vector
  if (option_exists(input, options[3])) {
    arguments.inputType = COLLATZ_PARITY_VECTOR;
    arguments.inputStr = get_cmd_option(input, options[3]);
    return;
  }

  /*helpPage();
  exit(0);*/
}
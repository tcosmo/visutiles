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

  bool trigger_help_page = true;

  // STDIN
  if (option_exists(input, options[OPTION_STDIN])) {
    arguments.inputType = STDIN;
    trigger_help_page = false;
  }

  // OPTION_NO_GUI
  if (option_exists(input, options[OPTION_NO_GUI])) {
    arguments.no_gui = true;
    trigger_help_page = false;
  }

  // OPTION_RUN_N_STEPS
  if (option_exists(input, options[OPTION_RUN_N_STEPS])) {
    std::string run_n_steps_str =
        get_cmd_option(input, options[OPTION_RUN_N_STEPS]);

    sscanf(run_n_steps_str.c_str(), "%zu", &arguments.run_n_steps);
    trigger_help_page = false;
  }

  if (trigger_help_page) {
    helpPage();
    exit(0);
  }
}

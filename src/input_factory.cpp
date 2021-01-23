#include "input_factory.h"

EdgeMap InputFactory::get_initial_configuration() {
  EdgeMap to_return;
  return to_return;
}

EdgeMap CollatzInputFactory::build_dummy_initial_configuration() {
  EdgeMap to_return;

  std::vector<PosEdge> edges_to_add;

  edges_to_add.push_back(std::make_pair(OrderedPosCouple({0, 0}, {1, 0}),
                                        std::make_pair(std::string("bin"), 0)));

  edges_to_add.push_back(std::make_pair(OrderedPosCouple({1, 0}, {1, 1}),
                                        std::make_pair(std::string("ter"), 1)));

  edges_to_add.push_back(std::make_pair(OrderedPosCouple({1, 1}, {0, 1}),
                                        std::make_pair(std::string("bin"), 1)));

  for (const PosEdge& pos_and_edge : edges_to_add) {
    to_return[pos_and_edge.first] = pos_and_edge.second;
  }

  return to_return;
}

EdgeMap CollatzInputFactory::build_parity_vector_initial_configuration() {
  EdgeMap to_return;
  return to_return;
}

EdgeMap CollatzInputFactory::get_initial_configuration() {
  EdgeMap to_return;

  switch (input_type) {
    case COLLATZ_PARITY_VECTOR:
      to_return = std::move(build_parity_vector_initial_configuration());
      break;
    default:
      to_return = std::move(build_dummy_initial_configuration());
      break;
  }

  return to_return;
}
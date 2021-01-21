#include "input_factory.h"

TileMap InputFactory::get_initial_configuration() {
  TileMap to_return;

  std::vector<std::pair<sf::Vector2i, TileId>> tiles_to_add;
  tiles_to_add.push_back({{0, 0}, 0});
  tiles_to_add.push_back({{0, 1}, 1});
  tiles_to_add.push_back({{1, 0}, 2});
  tiles_to_add.push_back({{1, 1}, 3});
  tiles_to_add.push_back({{-1, 0}, 4});
  tiles_to_add.push_back({{-2, 0}, 5});

  for (int i = 0; i < 20000; i += 1) {
    tiles_to_add.push_back({{2, i}, 5});
  }

  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile : tiles_to_add) {
    to_return[pos_and_tile.first] = pos_and_tile.second;
  }

  return to_return;
}

TileMap CollatzInputFactory::get_initial_configuration() {
  TileMap to_return;

  std::vector<std::pair<sf::Vector2i, TileId>> tiles_to_add;
  tiles_to_add.push_back({{0, 0}, 0});
  tiles_to_add.push_back({{0, 1}, 1});
  tiles_to_add.push_back({{1, 0}, 2});

  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile : tiles_to_add) {
    to_return[pos_and_tile.first] = pos_and_tile.second;
  }

  return to_return;
}
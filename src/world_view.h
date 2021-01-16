#pragma once

#include "global.h"
#include "world_controller.h"

#include <SFML/Graphics.hpp>

class WorldView : public sf::Drawable, public sf::Transformable {
 public:
  WorldView(const Tileset& tileset, const WorldController& w_controller);
  ~WorldView();

  void update();

 private:
  const Tileset& tileset;
  const WorldController& w_controller;

  sf::Texture tileset_skin;
  void load_tileset_skin();

  sf::VertexArray vertices;
  void add_tile_vertices(const sf::Vector2i& pos, const TileId& tile_id);
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::map<sf::Vector2i, bool, CompareTilesPositions> position_seen;
};
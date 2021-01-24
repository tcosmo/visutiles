#pragma once

#include "global.h"
#include "world.h"

#include <SFML/Graphics.hpp>

// Some graphic parameters
const static int GRAPHIC_EDGE_THICK = 5;
const static int GRAPHIC_EDGE_WIDTH = 64;
const static int GRAPHIC_TILE_SIZE = GRAPHIC_EDGE_WIDTH;
const static int GRAPHIC_EDGE_TEXT_SIZE = GRAPHIC_EDGE_WIDTH / 3;

class WorldView : public sf::Drawable, public sf::Transformable {
 public:
  WorldView(const Tileset& tileset, const World& world, const sf::Font& font);
  ~WorldView();

  void update();

  size_t get_vertex_count() { return vertex_count; };

 private:
  const Tileset& tileset;
  const World& world;

  sf::VertexBuffer vertex_buffer;
  size_t vertex_count;

  std::array<sf::Vertex, 4> get_edge_vertices(const PosEdge& pos_and_edge);
  std::array<sf::Vertex, 4> get_edge_char_vertices(const PosEdge& pos_and_edge);

  sf::Vector2f world_pos_to_screen_pos(const sf::Vector2i& pos);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::map<OrderedPosCouple, bool, CompareOrderedPosCouple> edge_seen;

  std::map<EdgeAlphabetName, sf::Color> alphabet_color;
  const sf::Font& font;
};
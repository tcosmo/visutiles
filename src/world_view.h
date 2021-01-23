#pragma once

#include "global.h"
#include "world_controller.h"

#include <SFML/Graphics.hpp>

class WorldView : public sf::Drawable, public sf::Transformable {
 public:
  WorldView(const Tileset& tileset, const WorldController& w_controller);
  ~WorldView();

  void update();

  size_t get_vertex_count() { return vertex_count; };

 private:
  const Tileset& tileset;
  const WorldController& w_controller;

  sf::VertexBuffer vertex_buffer;
  size_t vertex_count;

  std::array<sf::Vertex, 4> get_edge_vertices(const PosEdge& pos_and_edge);

  sf::Vector2f world_point_to_screen_point(const sf::Vector2i& pos);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::map<OrderedPosCouple, bool, CompareOrderedPosCouple> edge_seen;

  std::map<EdgeAlphabetName, sf::Color> alphabet_color;
};
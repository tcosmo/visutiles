#pragma once

#include "global.h"
#include "world.h"

#include <SFML/Graphics.hpp>
#include <numeric>

// Some graphic parameters
const static size_t GRAPHIC_EDGE_THICK = 5;
const static size_t GRAPHIC_EDGE_WIDTH = 64;
const static size_t GRAPHIC_TILE_SIZE = GRAPHIC_EDGE_WIDTH;
const static size_t GRAPHIC_TILE_TEXT_SIZE = GRAPHIC_EDGE_WIDTH / 2;
const static size_t GRAPHIC_EDGE_TEXT_SIZE = GRAPHIC_EDGE_WIDTH / 3;

// Layers
const static size_t NB_GRAPHIC_LAYERS = 4;
const static size_t LAYER_TILES_COLORS =
    0;  // Fill tiles with colors instead of text
const static size_t LAYER_EDGES = 1;
const static size_t LAYER_TILES = 2;
const static size_t LAYER_ADDITIONAL_TILES = 3;  // Dead tiles, versa tiles

class WorldView : public sf::Drawable, public sf::Transformable {
 public:
  WorldView(const Tileset& tileset, const World& world, const sf::Font& font);
  ~WorldView();

  void update();

  size_t get_total_vertex_count() {
    return std::accumulate(vertex_counts.begin(), vertex_counts.end(), 0);
  };

  void reset() {
    reset_vertex_buffer();
    edge_seen.clear();
    tile_seen.clear();
    tile_color_seen.clear();
  }

 private:
  const Tileset& tileset;
  const World& world;

  std::array<sf::VertexBuffer, NB_GRAPHIC_LAYERS> vertex_buffers;
  std::array<size_t, NB_GRAPHIC_LAYERS> vertex_counts;

  std::array<sf::Vertex, 4> get_edge_vertices(
      const EdgePosAndColor& pos_and_edge);
  std::array<sf::Vertex, 4> get_edge_char_vertices(
      const EdgePosAndColor& pos_and_edge);

  std::array<sf::Vertex, 4> get_tile_char_vertices(const TilePosAndName& tile);
  std::array<sf::Vertex, 4> get_tile_color_vertices(const TilePosAndName& tile);

  sf::Vector2f world_pos_to_screen_pos(const sf::Vector2i& pos);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  std::map<OrderedPosCouple, bool, CompareOrderedPosCouple> edge_seen;
  std::map<sf::Vector2i, bool, CompareTilesPositions> tile_seen;
  std::map<sf::Vector2i, bool, CompareTilesPositions> tile_color_seen;

  std::map<EdgeAlphabetName, sf::Color> alphabet_color;
  const sf::Font& font;

  void update_layer(size_t i_layer,
                    const std::vector<sf::Vertex> vertices_to_add);
  void update_edges_layer();
  void update_tiles_layer();
  void update_tiles_colors_layer();
  void reset_vertex_buffer();
};
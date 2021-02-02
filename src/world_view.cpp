#include "world_view.h"

WorldView::WorldView(const Tileset& tileset, const World& world,
                     const sf::Font& font)
    : tileset(tileset), world(world), font(font) {
  for (size_t i_layer = 0; i_layer < NB_GRAPHIC_LAYERS; i_layer += 1) {
    vertex_buffers[i_layer].setPrimitiveType(sf::Quads);
    vertex_buffers[i_layer].setUsage(sf::VertexBuffer::Usage::Dynamic);
    vertex_buffers[i_layer].create(VERTEX_BUFFER_MAX_SIZE);
    vertex_counts[i_layer] = 0;
  }

  for (size_t i_alphabet = 0; i_alphabet < tileset.get_alphabet_names().size();
       i_alphabet += 1)
    alphabet_color[tileset.get_alphabet_names()[i_alphabet]] =
        COLOR_WHEEL[i_alphabet % COLOR_WHEEL.size()];
}

void WorldView::reset_vertex_buffer() {
  sf::VertexBuffer tmp_vertex_buffer;
  tmp_vertex_buffer.setPrimitiveType(sf::Quads);
  tmp_vertex_buffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
  tmp_vertex_buffer.create(VERTEX_BUFFER_MAX_SIZE);

  for (size_t i_layer = 0; i_layer < NB_GRAPHIC_LAYERS; i_layer += 1) {
    vertex_buffers[i_layer].update(tmp_vertex_buffer);
    vertex_counts[i_layer] = 0;
  }
}

sf::Vector2f WorldView::world_pos_to_screen_pos(const sf::Vector2i& pos) {
  return {((float)GRAPHIC_TILE_SIZE) * ((float)pos.x),
          ((float)GRAPHIC_TILE_SIZE) * (-1 * ((float)pos.y))};
}

std::array<sf::Vertex, 4> WorldView::get_edge_vertices(
    const EdgePosAndColor& edge) {
  sf::Vector2i point_A = edge.pos.first;
  sf::Vector2i point_B = edge.pos.second;

  sf::Vector2f screen_point_A = world_pos_to_screen_pos(point_A);
  sf::Vector2f screen_point_B = world_pos_to_screen_pos(point_B);

  sf::Vector2f vect = screen_point_B - screen_point_A;
  sf::Vector2f normal_vect = get_normal_unit_vector(vect);

  sf::Vector2f screen_point_C =
      screen_point_B + GRAPHIC_EDGE_THICK * normal_vect;
  sf::Vector2f screen_point_D =
      screen_point_A + GRAPHIC_EDGE_THICK * normal_vect;

  std::array<sf::Vertex, 4> quad;
  // define its 4 corners
  quad[0].position = screen_point_A;
  quad[1].position = screen_point_B;
  quad[2].position = screen_point_C;
  quad[3].position = screen_point_D;

  quad[0].texCoords = {0, 0};
  quad[1].texCoords = {0, 0};
  quad[2].texCoords = {0, 0};
  quad[3].texCoords = {0, 0};

  quad[0].color = alphabet_color[edge.color.first];
  quad[1].color = alphabet_color[edge.color.first];
  quad[2].color = alphabet_color[edge.color.first];
  quad[3].color = alphabet_color[edge.color.first];

  return quad;
}

std::array<sf::Vertex, 4> WorldView::get_edge_char_vertices(
    const EdgePosAndColor& edge) {
  std::array<sf::Vertex, 4> text_quad;
  sf::Vector2i point_A = edge.pos.first;
  sf::Vector2i point_B = edge.pos.second;

  sf::Vector2f screen_point_A = world_pos_to_screen_pos(point_A);
  sf::Vector2f screen_point_B = world_pos_to_screen_pos(point_B);

  sf::Vector2f vect = screen_point_B - screen_point_A;
  vect /= Euclidean_norm(vect);
  sf::Vector2f normal_vect = -1 * get_normal_unit_vector(vect);

  // define text
  char edge_char = tileset.get_edge_char(edge.color);
  sf::Glyph glyph = font.getGlyph(edge_char, GRAPHIC_EDGE_TEXT_SIZE, false);

  // Centering text on edge
  sf::Vector2f mid_point =
      (screen_point_A + screen_point_B - GRAPHIC_EDGE_THICK * normal_vect) *
      (float)0.5;
  text_quad[0].position = mid_point -
                          glyph.bounds.height * (normal_vect * (float)0.5) -
                          glyph.bounds.width * (vect * (float)0.5);
  text_quad[1].position = text_quad[0].position + glyph.bounds.width * vect;
  text_quad[2].position =
      text_quad[1].position + glyph.bounds.height * normal_vect;
  text_quad[3].position =
      text_quad[0].position + glyph.bounds.height * normal_vect;

  sf::Vector2f top_left_text = {(float)glyph.textureRect.left,
                                (float)glyph.textureRect.top};
  sf::Vector2f text_size_vec = {(float)glyph.textureRect.width,
                                (float)glyph.textureRect.height};

  text_quad[0].texCoords = top_left_text;
  text_quad[1].texCoords = top_left_text + sf::Vector2f({1, 0}) * text_size_vec;

  text_quad[2].texCoords = top_left_text + text_size_vec;
  text_quad[3].texCoords = top_left_text + sf::Vector2f({0, 1}) * text_size_vec;

  return text_quad;
}

// Update the layer responsible for edges rendering
void WorldView::update_edges_layer() {
  const std::vector<EdgePosAndColor> newly_added_edges =
      world.get_newly_added_edges();

  if (newly_added_edges.size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const EdgePosAndColor& edge : newly_added_edges) {
    if (edge_seen.find(edge.pos) != edge_seen.end()) {
      warning_log(
          "Edge at position {(%d,%d),(%d,%d)} has already be drawn in the "
          "past.\n",
          edge.pos.first.x, edge.pos.first.y, edge.pos.second.x,
          edge.pos.second.y);
    }
    edge_seen[edge.pos] = true;

    std::array<sf::Vertex, 4> edge_vertices = get_edge_vertices(edge);
    for (const sf::Vertex& v : edge_vertices) vertices_to_add.push_back(v);

    std::array<sf::Vertex, 4> edge_char_vertices = get_edge_char_vertices(edge);
    for (const sf::Vertex& v : edge_char_vertices) vertices_to_add.push_back(v);
  }

  update_layer(LAYER_EDGES, vertices_to_add);
}

std::array<sf::Vertex, 4> WorldView::get_tile_char_vertices(
    const TilePosAndName& tile) {
  // define text
  char tile_char = tile.name;
  sf::Glyph glyph = font.getGlyph(tile_char, GRAPHIC_TILE_TEXT_SIZE, false);

  sf::RectangleShape glyph_rectangle;
  glyph_rectangle.setSize(
      sf::Vector2f(glyph.bounds.width, glyph.bounds.height));
  glyph_rectangle.setOrigin(glyph_rectangle.getSize() * (float)0.5);

  sf::Vector2f tile_center =
      (world_pos_to_screen_pos(tile.pos) +
       world_pos_to_screen_pos(tile.pos + WORLD_NORTH + WORLD_WEST)) *
      (float)0.5;
  glyph_rectangle.setPosition(tile_center);

  std::array<sf::Vertex, 4> text_quad;
  sf::Vector2f top_left = {glyph_rectangle.getGlobalBounds().left,
                           glyph_rectangle.getGlobalBounds().top};
  text_quad[0].position = top_left;

  text_quad[1].position =
      top_left + glyph_rectangle.getSize() * sf::Vector2f({1, 0});
  text_quad[2].position =
      top_left + glyph_rectangle.getSize() * sf::Vector2f({1, 1});
  text_quad[3].position =
      top_left + glyph_rectangle.getSize() * sf::Vector2f({0, 1});

  sf::Vector2f top_left_text = {(float)glyph.textureRect.left,
                                (float)glyph.textureRect.top};
  sf::Vector2f text_size_vec = {(float)glyph.textureRect.width,
                                (float)glyph.textureRect.height};

  text_quad[0].texCoords = top_left_text;
  text_quad[1].texCoords = top_left_text + sf::Vector2f({1, 0}) * text_size_vec;

  text_quad[2].texCoords = top_left_text + text_size_vec;
  text_quad[3].texCoords = top_left_text + sf::Vector2f({0, 1}) * text_size_vec;

  return text_quad;
}

// Update the layer responsible for tiles rendering
void WorldView::update_tiles_layer() {
  const std::vector<TilePosAndName> newly_added_completed_tiles =
      world.get_newly_added_completed_tiles();

  if (newly_added_completed_tiles.size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const TilePosAndName& tile : newly_added_completed_tiles) {
    if (tile_seen.find(tile.pos) != tile_seen.end()) {
      warning_log(
          "Tile at position (%d,%d) has already be drawn in the past.\n",
          tile.pos.x, tile.pos.y);
    }
    tile_seen[tile.pos] = true;

    std::array<sf::Vertex, 4> tile_char_vertices = get_tile_char_vertices(tile);
    for (const sf::Vertex& v : tile_char_vertices) vertices_to_add.push_back(v);
  }

  update_layer(LAYER_TILES, vertices_to_add);
}

std::array<sf::Vertex, 4> WorldView::get_tile_color_vertices(
    const TilePosAndName& tile) {
  char tile_char = tile.name;
  sf::Color tile_color =
      COLOR_WHEEL_SECONDARY[(tile_char + 4) % COLOR_WHEEL_SECONDARY.size()];
  tile_color.a = 120;
  std::array<sf::Vertex, 4> text_quad;
  text_quad[0].position = world_pos_to_screen_pos(tile.pos);

  text_quad[1].position = world_pos_to_screen_pos(tile.pos + WORLD_NORTH);
  text_quad[2].position =
      world_pos_to_screen_pos(tile.pos + WORLD_NORTH + WORLD_WEST);
  text_quad[3].position = world_pos_to_screen_pos(tile.pos + WORLD_WEST);

  text_quad[0].color = tile_color;
  text_quad[1].color = tile_color;

  text_quad[2].color = tile_color;
  text_quad[3].color = tile_color;

  return text_quad;
}

// Update the layer responsible for tiles rendering in color mode
void WorldView::update_tiles_colors_layer() {
  const std::vector<TilePosAndName> newly_added_completed_tiles =
      world.get_newly_added_completed_tiles();

  if (newly_added_completed_tiles.size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const TilePosAndName& tile : newly_added_completed_tiles) {
    if (tile_color_seen.find(tile.pos) != tile_color_seen.end()) {
      warning_log(
          "Tile at position (%d,%d) has already be drawn in the past.\n",
          tile.pos.x, tile.pos.y);
    }
    tile_color_seen[tile.pos] = true;

    std::array<sf::Vertex, 4> tile_char_vertices =
        get_tile_color_vertices(tile);
    for (const sf::Vertex& v : tile_char_vertices) vertices_to_add.push_back(v);
  }

  update_layer(LAYER_TILES, vertices_to_add);
}

void WorldView::update_layer(size_t i_layer,
                             const std::vector<sf::Vertex> vertices_to_add) {
  if (i_layer >= NB_GRAPHIC_LAYERS) return;

  if (vertex_counts[i_layer] + vertices_to_add.size() >
      VERTEX_BUFFER_MAX_SIZE) {
    warning_log(
        "The maximum number of drawable vertices, `%d`, has been reached, new "
        "updates are ignored. You can increase the value of "
        "`VERTEX_BUFFER_MAX_SIZE` in the file `config.h.in`.",
        VERTEX_BUFFER_MAX_SIZE);
    return;
  }

  vertex_buffers[i_layer].update(&vertices_to_add[0], vertices_to_add.size(),
                                 vertex_counts[i_layer]);
  vertex_counts[i_layer] += vertices_to_add.size();
}

void WorldView::update() {
  update_edges_layer();
  update_tiles_colors_layer();
  update_tiles_layer();
}

void WorldView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // apply the transform
  states.transform *= getTransform();

  // draw the vertex array
  // target.draw(vertices, states);
  for (size_t i_layer = 0; i_layer < NB_GRAPHIC_LAYERS; i_layer += 1) {
    // apply the layer's texture
    if (i_layer == LAYER_EDGES) {
      states.texture = &font.getTexture(GRAPHIC_EDGE_TEXT_SIZE);
    } else if (i_layer == LAYER_TILES) {
      states.texture = &font.getTexture(GRAPHIC_TILE_TEXT_SIZE);
    }
    target.draw(vertex_buffers[i_layer], states);
  }
}

WorldView::~WorldView() {}
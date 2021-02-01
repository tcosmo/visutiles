#include "world_view.h"

WorldView::WorldView(const Tileset& tileset, const World& world,
                     const sf::Font& font)
    : tileset(tileset), world(world), font(font) {
  vertex_buffer.setPrimitiveType(sf::Quads);
  vertex_buffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
  vertex_buffer.create(VERTEX_BUFFER_MAX_SIZE);

  vertex_count = 0;

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

  vertex_buffer.update(tmp_vertex_buffer);
  vertex_count = 0;
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

void WorldView::update() {
  const std::vector<EdgePosAndColor> newly_added_edges =
      world.get_newly_added_edges();
  if (newly_added_edges.size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const EdgePosAndColor& edge : newly_added_edges) {
    if (edge_seen.find(edge.pos) != edge_seen.end()) {
      warning_log("Edge {(%d,%d),(%d,%d)} has already be drawn in the past.\n",
                  edge.pos.first.x, edge.pos.first.y, edge.pos.second.x,
                  edge.pos.second.y);
    }
    edge_seen[edge.pos] = true;

    std::array<sf::Vertex, 4> edge_vertices = get_edge_vertices(edge);
    for (const sf::Vertex& v : edge_vertices) vertices_to_add.push_back(v);

    std::array<sf::Vertex, 4> edge_char_vertices = get_edge_char_vertices(edge);
    for (const sf::Vertex& v : edge_char_vertices) vertices_to_add.push_back(v);
  }

  if (vertex_count + vertices_to_add.size() > VERTEX_BUFFER_MAX_SIZE) {
    warning_log(
        "The maximum number of drawable vertices, `%d`, has been reached, new "
        "updates are ignored. You can increase the value of "
        "`VERTEX_BUFFER_MAX_SIZE` in the file `config.h.in`.",
        VERTEX_BUFFER_MAX_SIZE);
    return;
  }

  vertex_buffer.update(&vertices_to_add[0], vertices_to_add.size(),
                       vertex_count);
  vertex_count += vertices_to_add.size();
}

void WorldView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // apply the transform
  states.transform *= getTransform();

  // apply the tileset texture
  states.texture = &font.getTexture(GRAPHIC_EDGE_TEXT_SIZE);
  // draw the vertex array
  // target.draw(vertices, states);
  target.draw(vertex_buffer, states);
}

WorldView::~WorldView() {}
#include "world_view.h"

WorldView::WorldView(const Tileset& tileset,
                     const WorldController& w_controller, const sf::Font& font)
    : tileset(tileset), w_controller(w_controller), font(font) {
  vertex_buffer.setPrimitiveType(sf::Quads);
  vertex_buffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
  vertex_buffer.create(VERTEX_BUFFER_MAX_SIZE);
  vertex_count = 0;

  for (size_t i_alphabet = 0; i_alphabet < tileset.get_alphabet_names().size();
       i_alphabet += 1)
    alphabet_color[tileset.get_alphabet_names()[i_alphabet]] =
        COLOR_WHEEL[i_alphabet % COLOR_WHEEL.size()];
}

sf::Vector2f WorldView::world_point_to_screen_point(const sf::Vector2i& pos) {
  return {((float)GRAPHIC_TILE_SIZE) * ((float)pos.x),
          ((float)GRAPHIC_TILE_SIZE) * (-1 * ((float)pos.y))};
}

std::array<sf::Vertex, 8> WorldView::get_edge_vertices(
    const PosEdge& pos_and_edge) {
  sf::Vector2i point_A = pos_and_edge.first.first;
  sf::Vector2i point_B = pos_and_edge.first.second;

  sf::Vector2f screen_point_A = world_point_to_screen_point(point_A);
  sf::Vector2f screen_point_B = world_point_to_screen_point(point_B);

  sf::Vector2f vect = screen_point_B - screen_point_A;
  sf::Vector2f normal_vect = get_normal_unit_vector(vect);

  sf::Vector2f screen_point_C =
      screen_point_B + GRAPHIC_EDGE_THICK * normal_vect;
  sf::Vector2f screen_point_D =
      screen_point_A + GRAPHIC_EDGE_THICK * normal_vect;

  std::array<sf::Vertex, 8> quad_and_text;
  // define its 4 corners
  quad_and_text[0].position = screen_point_A;
  quad_and_text[1].position = screen_point_B;
  quad_and_text[2].position = screen_point_C;
  quad_and_text[3].position = screen_point_D;

  quad_and_text[0].texCoords = {0, 0};
  quad_and_text[1].texCoords = {0, 0};
  quad_and_text[2].texCoords = {0, 0};
  quad_and_text[3].texCoords = {0, 0};

  quad_and_text[0].color = alphabet_color[pos_and_edge.second.first];
  quad_and_text[1].color = alphabet_color[pos_and_edge.second.first];
  quad_and_text[2].color = alphabet_color[pos_and_edge.second.first];
  quad_and_text[3].color = alphabet_color[pos_and_edge.second.first];

  // define text
  char edge_char = tileset.get_edge_char(pos_and_edge.second);
  sf::Glyph glyph = font.getGlyph(edge_char, GRAPHIC_EDGE_TEXT_SIZE, false);

  sf::Vector2f mid_point = (screen_point_A + screen_point_B) * (float)0.5;
  quad_and_text[4].position = mid_point;
  quad_and_text[5].position = mid_point + sf::Vector2f{glyph.bounds.width, 0};
  quad_and_text[6].position =
      mid_point + sf::Vector2f{glyph.bounds.width, glyph.bounds.height};
  quad_and_text[7].position = mid_point + sf::Vector2f{0, glyph.bounds.height};

  sf::Vector2f top_left_text = {(float)glyph.textureRect.left,
                                (float)glyph.textureRect.top};
  sf::Vector2f text_size_vec = {(float)glyph.textureRect.width,
                                (float)glyph.textureRect.height};

  quad_and_text[4].texCoords = top_left_text;
  quad_and_text[5].texCoords =
      top_left_text + sf::Vector2f({1, 0}) * text_size_vec;

  quad_and_text[6].texCoords = top_left_text + text_size_vec;
  quad_and_text[7].texCoords =
      top_left_text + sf::Vector2f({0, 1}) * text_size_vec;

  return quad_and_text;
}

void WorldView::update() {
  if (w_controller.get_newly_added_edges().size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const PosEdge& pos_and_edge : w_controller.get_newly_added_edges()) {
    OrderedPosCouple edge = pos_and_edge.first;
    if (edge_seen.find(edge) != edge_seen.end()) {
      warning_log("Edge {(%d,%d),(%d,%d)} has already be drawn in the past.\n",
                  edge.first.x, edge.first.y, edge.second.x, edge.second.y);
    }
    edge_seen[edge] = true;

    std::array<sf::Vertex, 8> tile_vertices = get_edge_vertices(pos_and_edge);

    for (sf::Vertex v : tile_vertices) vertices_to_add.push_back(v);
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
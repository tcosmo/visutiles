#include "world_view.h"

WorldView::WorldView(const Tileset& tileset,
                     const WorldController& w_controller)
    : tileset(tileset), w_controller(w_controller) {
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

std::array<sf::Vertex, 4> WorldView::get_edge_vertices(
    const PosEdge& pos_and_edge) {
  sf::Vector2i point_A = pos_and_edge.first.first;
  sf::Vector2i point_B = pos_and_edge.first.second;

  sf::Vector2f screen_point_A = world_point_to_screen_point(point_A);
  sf::Vector2f screen_point_B = world_point_to_screen_point(point_B);

  sf::Vector2f normal_vect =
      get_normal_unit_vector(screen_point_B - screen_point_A);

  sf::Vector2f screen_point_C = screen_point_B + normal_vect;
  sf::Vector2f screen_point_D = screen_point_A + normal_vect;

  std::array<sf::Vertex, 4> quad;
  // define its 4 corners
  quad[0].position = screen_point_A;
  quad[1].position = screen_point_B;
  quad[2].position = screen_point_C;
  quad[3].position = screen_point_D;

  quad[0].color = alphabet_color[pos_and_edge.second.first];
  quad[1].color = alphabet_color[pos_and_edge.second.first];
  quad[2].color = alphabet_color[pos_and_edge.second.first];
  quad[3].color = alphabet_color[pos_and_edge.second.first];

  return quad;
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

    std::array<sf::Vertex, 4> tile_vertices = get_edge_vertices(pos_and_edge);

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
  // states.texture = &tileset_skin;
  // draw the vertex array
  // target.draw(vertices, states);
  target.draw(vertex_buffer, states);
}

WorldView::~WorldView() {}
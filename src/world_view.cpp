#include "world_view.h"

WorldView::WorldView(const Tileset& tileset,
                     const WorldController& w_controller)
    : tileset(tileset), w_controller(w_controller) {
  vertex_buffer.setPrimitiveType(sf::Quads);
  vertex_buffer.setUsage(sf::VertexBuffer::Usage::Dynamic);
  vertex_buffer.create(VERTEX_BUFFER_MAX_SIZE);
  vertex_count = 0;

  load_tileset_skin();
  setRotation(tileset.rotation);
  setScale({tileset.scale_x, tileset.scale_y});
}

std::array<sf::Vertex, 4> WorldView::get_tile_vertices(const sf::Vector2i& pos,
                                                       const TileId& tile_id) {
  std::array<sf::Vertex, 4> quad;

  sf::Vector2f top_left = {(float)(pos.x * ((float)tileset.tile_width)),
                           (float)(-1 * pos.y * ((float)tileset.tile_height))};
  sf::Vector2f one_x = {(float)tileset.tile_width, 0};
  sf::Vector2f one_y = {0, (float)tileset.tile_height};

  // define its 4 corners
  quad[0].position = top_left;
  quad[1].position = top_left + one_x;
  quad[2].position = top_left + one_x + one_y;
  quad[3].position = top_left + one_y;

  // define its 4 texture coordinates
  quad[0].texCoords = {(float)tileset.tile_width * tile_id, 0};
  quad[1].texCoords = {(float)tileset.tile_width * (tile_id + 1), 0};
  quad[2].texCoords = {(float)tileset.tile_width * (tile_id + 1),
                       (float)tileset.tile_height};
  quad[3].texCoords = {(float)tileset.tile_width * tile_id,
                       (float)tileset.tile_height};

  return quad;
}

void WorldView::update() {
  if (w_controller.get_newly_added_tiles().size() == 0) return;

  std::vector<sf::Vertex> vertices_to_add;
  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile :
       w_controller.get_newly_added_tiles()) {
    sf::Vector2i pos = pos_and_tile.first;
    if (position_seen.find(pos) != position_seen.end()) {
      warning_log("Position (%d,%d) has already be drawn in the past.\n", pos.x,
                  pos.y);
    }
    position_seen[pos] = true;

    std::array<sf::Vertex, 4> tile_vertices =
        get_tile_vertices(pos, pos_and_tile.second);

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

void WorldView::load_tileset_skin() {
  const std::string& tileset_skin_name = tileset.tilset_skin;

  std::string tile_skin_path =
      tileset.tsx_file_dir + OS_FILE_SEPARATOR + tileset_skin_name;

  if (!tileset_skin.loadFromFile(tile_skin_path)) {
    fatal_error_log("Could not load tile skin `%s`. Abort.",
                    tile_skin_path.c_str());
  }
}

void WorldView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // apply the transform
  states.transform *= getTransform();

  // apply the tileset texture
  states.texture = &tileset_skin;
  // draw the vertex array
  // target.draw(vertices, states);
  target.draw(vertex_buffer, states);
}

WorldView::~WorldView() {}
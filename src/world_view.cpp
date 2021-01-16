#include "world_view.h"

WorldView::WorldView(const Tileset& tileset,
                     const WorldController& w_controller)
    : tileset(tileset), w_controller(w_controller) {
  vertices.setPrimitiveType(sf::Quads);
  load_tileset_skin();
  setRotation(tileset.rotation);
}

void WorldView::add_tile_vertices(const sf::Vector2i& pos,
                                  const TileId& tile_id) {
  sf::Vertex quad[4];

  sf::Vector2f top_left = {(float)(pos.x * 200), (float)(-1 * pos.y * 200)};
  sf::Vector2f one_x = {200, 0};
  sf::Vector2f one_y = {0, 200};

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

  vertices.append(quad[0]);
  vertices.append(quad[1]);
  vertices.append(quad[2]);
  vertices.append(quad[3]);
}

void WorldView::update() {
  for (const std::pair<sf::Vector2i, TileId>& pos_and_tile :
       w_controller.newly_added_tiles) {
    sf::Vector2i pos = pos_and_tile.first;
    if (position_seen.find(pos) != position_seen.end()) {
      warning_log("Position (%d,%d) has already be drawn in the past.", pos.x,
                  pos.y);
    }
    position_seen[pos] = true;

    add_tile_vertices(pos, pos_and_tile.second);
  }
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
  target.draw(vertices, states);
}

WorldView::~WorldView() {}
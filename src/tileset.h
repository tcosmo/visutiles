#pragma once

#include <map>
#include <string>
#include <vector>

#include "third_party/tinyxml/tinyxml2.h"

#include "global.h"

#include "square_grid.h"

// Valid colors are >= 0 ids. Value -1 can be used to mean "no color".
#define WANG_NO_COLOR -1
typedef int WangColor;

typedef int TileId;

// Tiles on the square grid with Wang colors on edges
struct Tile {
  Tile() {
    for (size_t i_dir = 0; i_dir < SQUARE_GRID_NEIGHBORING_SIZE; i_dir += 1)
      wang_edges[i_dir] = WANG_NO_COLOR;
  }
  Tile(TileId id,
       std::array<WangColor, SQUARE_GRID_NEIGHBORING_SIZE> wang_edges,
       std::string type, uint32_t skin_width, uint32_t skin_height)
      : wang_edges(wang_edges),
        type(type),
        skin_width(skin_width),
        skin_height(skin_height) {}

  TileId id;
  // Wang color on each side of the tile in order West, South, East, North
  std::array<WangColor, SQUARE_GRID_NEIGHBORING_SIZE> wang_edges;
  // The tile type is just a name/comment that user can define in
  // the .tsx interface.
  std::string type;
  uint32_t skin_width;
  uint32_t skin_height;
};

// Represents a tileset, we expect one skin file per tile with a .tsx
// file which summarises tileset information (tiles and Wang colors)
// .tsx files are produced by free open source software "Tiled Map Editor"
// Cf: https://www.mapeditor.org/
class Tileset {
 public:
  Tileset(std::string tsx_file_dir, std::string tsx_filename);
  ~Tileset();

  // Returns the set of ids of tiles which satisfies having specific color on
  // their edges in order West, South, East, North. If one of these colors is -1
  // then no constraints is enforced on that side.
  std::vector<TileId> Wang_query(
      std::array<WangColor, SQUARE_GRID_NEIGHBORING_SIZE> color_constraints);

  // We use map and not vector to match with tile ids which are
  // specified in .tsx file and used by Wang tiles.
  std::map<TileId, Tile> tiles;
  std::vector<WangColor> wang_colors;
  uint32_t tile_count;
  std::string tileset_name;
  uint32_t tile_width, tile_height;
  uint32_t tileset_width, tileset_height;

  std::string tsx_filename;
  std::string tsx_file_dir;
  std::string tsx_file_path;

  std::string tilset_skin;

  // Rotation common to all tiles. For instance 45° in the case of
  // Collatz tileset.
  float rotation;
  float scale_x;
  float scale_y;

 private:
  void parse_tsx_file();
  void tsx_extract_tiles(tinyxml2::XMLElement* root);
  void tsx_extract_Wang_colors(tinyxml2::XMLElement* root);

  std::map<std::array<WangColor, SQUARE_GRID_NEIGHBORING_SIZE>,
           std::vector<TileId>>
      memoize_queries;

  tinyxml2 ::XMLDocument tsx_xml_doc;
};
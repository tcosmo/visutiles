#include "tileset.h"

Tileset::Tileset(std::string tsx_file_dir, std::string tsx_filename)
    : tsx_file_dir(tsx_file_dir), tsx_filename(tsx_filename) {
  rotation = 0.0;
  scale_x = 1.0;
  scale_y = 1.0;
  parse_tsx_file();
}

Tileset::~Tileset() {}

void Tileset::tsx_extract_tiles(tinyxml2::XMLElement* root) {
  for (TileId t_id = 0; t_id < tile_count; t_id += 1) {
    Tile new_tile;
    new_tile.id = t_id;
    tiles[new_tile.id] = new_tile;
  }

  for (tinyxml2::XMLElement* tile_elem = root->FirstChildElement("tile");
       tile_elem != NULL; tile_elem = tile_elem->NextSiblingElement("tile")) {
    TileId t_id = std::stoi(tile_elem->Attribute("id"));
    tiles[t_id].type = tile_elem->Attribute("type");
  }
}

void Tileset::tsx_extract_Wang_colors(tinyxml2::XMLElement* root) {
  tinyxml2::XMLElement* wangsets = root->FirstChildElement("wangsets");
  if (!wangsets) {
    fatal_error_log("No wangset defined in `%s`. Abort. \n",
                    tsx_file_path.c_str());
  }

  uint32_t wangset_count = 0;
  for (tinyxml2::XMLElement* wangset = wangsets->FirstChildElement("wangset");
       wangset != NULL; wangset = wangset->NextSiblingElement("wangset")) {
    if (wangset_count >= 1) {
      warning_log(
          "Multiple wangsets are defined in `%s`. Only the first one "
          "is considered. \n",
          tsx_file_path.c_str());
      break;
    }

    if (wangset->FirstChildElement("wangcornercolor")) {
      warning_log(
          "A Wang corner color is defined in `%s`. Corner colors are "
          "ignored in %s. \n",
          tsx_file_path.c_str(), visutiles_PROG_NAME);
    }

    /* Extracting colors */
    WangColor color = 0;
    for (tinyxml2::XMLElement* wangcolor =
             wangset->FirstChildElement("wangedgecolor");
         wangcolor != NULL;
         wangcolor = wangcolor->NextSiblingElement("wangcolor")) {
      wang_colors.push_back(color);
      color += 1;
    }

    /* Extracting tiles edge colors */
    for (tinyxml2::XMLElement* wangtile =
             wangset->FirstChildElement("wangtile");
         wangtile != NULL;
         wangtile = wangtile->NextSiblingElement("wangtile")) {
      uint32_t tile_id = std::stoi(wangtile->Attribute("tileid"));
      std::string wang_id = std::string(wangtile->Attribute("wangid"));
      // We skip one character over two of the tsx wang id string as
      // we ignore Wang corner colors.
      size_t i_dir = 0;
      for (size_t i_char = 2; i_char < wang_id.size(); i_char += 2) {
        char hexchar = wang_id[i_char];
        uint32_t color_id =
            (hexchar >= 'A') ? (hexchar - 'A' + 10) : (hexchar - '0');
        color_id -= 1;  // tsx file encodes wang colors id starting at 1, 0
                        // means no color
        assert(i_dir < SQUARE_GRID_NEIGHBORING_SIZE);
        tiles[tile_id].wang_edges[i_dir] = color_id;
        i_dir += 1;
      }
    }

    wangset_count += 1;
  }

  if (wangset_count == 0) {
    fatal_error_log("No wangset defined in `%s`. Abort. \n",
                    tsx_file_path.c_str());
  }
}

void Tileset::parse_tsx_file() {
  // TODO: in this parsing rooting std::stoi is used a lot, it throws exception
  // if asked to translate a non integer string to int. These exception are
  // ignored for the moment but will cause program to crash.

  tsx_file_path = tsx_file_dir + OS_FILE_SEPARATOR + tsx_filename;
  tinyxml2::XMLError xml_err = tsx_xml_doc.LoadFile(tsx_file_path.c_str());

  if (xml_err != tinyxml2::XML_SUCCESS) {
    fatal_error_log("Cannot open tileset file `%s`. TinyXML error %d. Abort.\n",
                    tsx_file_path.c_str(), xml_err);
  }

  tinyxml2::XMLElement* root = tsx_xml_doc.RootElement();
  if (!root) {
    fatal_error_log("Cannot open tileset file `%s`. TinyXML error %d. Abort.\n",
                    tsx_file_path.c_str(), xml_err);
  }

  tile_count = std::stoi(root->Attribute("tilecount"));
  tileset_name = std::string(root->Attribute("name"));
  tile_width = std::stoi(root->Attribute("tilewidth"));
  tile_height = std::stoi(root->Attribute("tileheight"));

  // Find rotation property (if set)

  if (root->FirstChildElement("properties")) {
    for (tinyxml2::XMLElement* property = root->FirstChildElement("properties")
                                              ->FirstChildElement("property");
         property != NULL;
         property = property->NextSiblingElement("property")) {
      if (std::string(property->Attribute("name")) == "rotation") {
        rotation = std::stof(property->Attribute("value"));
      }
      if (std::string(property->Attribute("name")) == "scale_x") {
        scale_x = std::stof(property->Attribute("value"));
      }
      if (std::string(property->Attribute("name")) == "scale_y") {
        scale_y = std::stof(property->Attribute("value"));
      }
    }
  }

  if (!root->FirstChildElement("image")) {
    fatal_error_log("No source image is given in tilset file `%s`. Abort.\n",
                    tsx_file_path.c_str());
  }

  tilset_skin = root->FirstChildElement("image")->Attribute("source");
  tileset_width =
      std::stoi(root->FirstChildElement("image")->Attribute("width"));
  tileset_height =
      std::stoi(root->FirstChildElement("image")->Attribute("height"));

  tsx_extract_tiles(root);
  assert(tile_count == tiles.size());

  tsx_extract_Wang_colors(root);
}

std::vector<TileId> Tileset::Wang_query(
    std::array<WangColor, SQUARE_GRID_NEIGHBORING_SIZE> color_constraints) {
  if (memoize_queries.find(color_constraints) != memoize_queries.end())
    return memoize_queries[color_constraints];

  std::vector<TileId> to_return;

  for (std::pair<size_t, const Tile&> id_and_tile : tiles) {
    bool tile_matches_constraints = true;
    for (int i_side = 0; i_side < SQUARE_GRID_NEIGHBORING_SIZE; i_side += 1) {
      if (color_constraints[i_side] == -1) continue;
      if (color_constraints[i_side] != id_and_tile.second.wang_edges[i_side]) {
        tile_matches_constraints = false;
        break;
      }
    }
    if (tile_matches_constraints) {
      to_return.push_back(id_and_tile.first);
    }
  }

  memoize_queries[color_constraints] = to_return;

  return to_return;
}
// clang-format off
#define visutiles_VERSION_MAJOR 0
#define visutiles_VERSION_MINOR 1
#define visutiles_VERSION_PATCH 2
// clang-format on

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define VERSION                      \
  STRINGIFY(visutiles_VERSION_MAJOR) \
  "." STRINGIFY(visutiles_VERSION_MINOR) "." STRINGIFY(visutiles_VERSION_PATCH)

#define visutiles_PROG_NAME "VisuTiles"
#define visutiles_PROG_NAME_EXEC "visutiles"
#define TARGET_FPS 30

// Maximum number of vertices that can be drawn simultaneously on the screen
// Divide by 4 to get the corresponding number of tiles for square tiles with
// 4 vertices.
#define VERTEX_BUFFER_MAX_SIZE 20000

#include "graphic_engine.h"

GraphicEngine::GraphicEngine(WorldController& world_controller,
                             WorldView& world_view, const Tileset& tileset,
                             int screen_w, int screen_h)
    : world_controller(world_controller),
      world_view(world_view),
      tileset(tileset) {
  window.create(sf::VideoMode(screen_w, screen_h), visutiles_PROG_NAME);
  window.setFramerateLimit(TARGET_FPS);

  camera.set_translation_vector(
      {(float)tileset.tile_width, (float)tileset.tile_height});

  camera.init_view(window);
  window.setView(camera.camera_view);
}

GraphicEngine::~GraphicEngine() {}

void GraphicEngine::print_simulation_report() {
  printf("=== Simulation Report ===\n");
  printf("   == Tiles ==\n");
  printf("      - Total number of tiles: %zu\n",
         world_controller.get_tile_count());
  printf("      - World vertex count: %zu/%d\n", world_view.get_vertex_count(),
         VERTEX_BUFFER_MAX_SIZE);
}

void GraphicEngine::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        camera.handle_camera_events(event);
        switch (event.key.code) {
          case sf::Keyboard::A:
            print_simulation_report();
            break;

          case sf::Keyboard::Escape:
            window.close();
            break;
        }
      }

      if (event.type == sf::Event::Closed) window.close();
    }

    if (camera.has_changed) {
      window.setView(camera.camera_view);
      camera.has_changed = false;
    }
    window.clear(sf::Color(0.2 * 255, 0.2 * 255, 0.2 * 255));
    window.draw(world_view);
    window.display();
  }
}
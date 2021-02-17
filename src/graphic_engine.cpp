#include "graphic_engine.h"

GraphicEngine::GraphicEngine(World& world, WorldView& world_view,
                             const Tileset& tileset, int screen_w, int screen_h)
    : world(world), world_view(world_view), tileset(tileset) {
  window.create(sf::VideoMode(screen_w, screen_h), visutiles_PROG_NAME);
  window.setFramerateLimit(TARGET_FPS);
  camera_init();
}

GraphicEngine::~GraphicEngine() {}

void GraphicEngine::print_simulation_report() {
  printf("=== Simulation Report ===\n");
  printf("   == Edges ==\n");
  printf("      - Edges count: %zu\n", world.edge_count());
  printf("      - Missmatching edges count: %zu\n",
         world.get_mismatching_edge_count());
  printf("\n   == Tiles ==\n");
  printf("      - Uncompleted tiles: %zu\n", world.uncompleted_tile_count());
  printf("      - Completed tiles: %zu\n", world.completed_tile_count());
  printf("      - Dead tiles: %zu\n", world.dead_tile_count());

  printf("\n   == Graphics ==\n");

  printf("      - World vertex count: %zu/%d\n",
         world_view.get_total_vertex_count(), VERTEX_BUFFER_MAX_SIZE);
}

bool GraphicEngine::is_ctrl_pressed() {
  return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
}

bool GraphicEngine::is_shift_pressed() {
  return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
}

bool GraphicEngine::is_alt_pressed() {
  return sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);
}

void GraphicEngine::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      handle_camera_events(event);
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::A:
            print_simulation_report();
            break;

          case sf::Keyboard::N:
            world.update();
            world_view.update();
            break;

          case sf::Keyboard::R:
            world.reset();
            world_view.reset();
            world_view.update();
            break;

          case sf::Keyboard::Escape:
            window.close();
            break;
        }
      }

      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear(sf::Color(0.2 * 255, 0.2 * 255, 0.2 * 255));
    window.draw(world_view);
    window.display();
  }
}
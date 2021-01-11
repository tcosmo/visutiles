#include "graphic_engine.h"

GraphicEngine::GraphicEngine(World &world, int screen_w, int screen_h)
    : world(world) {
  window.create(sf::VideoMode(screen_w, screen_h), visutiles_PROG_NAME);
  window.setFramerateLimit(TARGET_FPS);

  camera = window.getDefaultView();
  window.setView(camera);
  currentZoom = 1.0;
}

GraphicEngine::~GraphicEngine() {}

void GraphicEngine::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;
        }
      }

      if (event.type == sf::Event::Closed) window.close();
    }

    window.clear(sf::Color::Black);
    window.display();
  }
}
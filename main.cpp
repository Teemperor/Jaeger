#include <Character.h>
#include <Logger.h>
#include <PathFinder.h>
#include <SFML/Graphics.hpp>
#include <controls/PlayerControls.h>
#include <fstream>
#include <gamedata/GameData.h>
#include <iostream>
#include <level/Level.h>
#include <level/LevelGen.h>
#include <level/WorldGen.h>
#include <stdlib.h>
#include <unordered_map>

int main() {
  Logger mainLogger;

  srand(2);


  /** Prepare the window */
  sf::RenderWindow Window(sf::VideoMode::getDesktopMode(), "Test");

  Window.setFramerateLimit(60);

  sf::View view = Window.getView();
  view.zoom(0.3f);

  GameData Data(".");

  WorldGen gen;
  World *world = gen.generate(Data);

  Level &level = *world->getLevels().at(0);

  Character *player1 = new Character(level, Vec2(45 * 16, 42 * 16));

  Character *player2 = new Character(level, Vec2(680, 680));

  for (int i = 0; i < 2; i++) {
    Vec2 pos = Vec2(200 + rand() % 700, 200 + rand() % 700);
    if (level.passable(pos))
      new Character(level, pos);
  }

  sf::Vector2f viewCenter;

  PlayerControls controls1(1);
  player1->setPlayerControls(&controls1);

  PlayerControls controls2(2);
  player2->setPlayerControls(&controls2);

  sf::Clock clock;

  while (Window.isOpen()) {
    std::vector<sf::Event> events;
    sf::Event event;
    while (Window.pollEvent(event)) {
      events.push_back(event);
      if (event.type == sf::Event::EventType::Closed) {
        return 0;
      }
      if (event.type == sf::Event::MouseWheelScrolled) {
        view.zoom(1 - event.mouseWheelScroll.delta / 30.0f);
      }
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape)
        return 0;

      if (event.type == sf::Event::Resized) {
        // update the view to the new size of the window
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        Window.setView(sf::View(visibleArea));
        view = Window.getView();
        view.zoom(0.3f);
      }
    }
    viewCenter.x = player1->getPos().getX();
    viewCenter.y = player1->getPos().getY();

    controls1.update();
    controls2.update();

    sf::Time elapsed = clock.restart();
    world->update(elapsed.asSeconds());

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      viewCenter.x = sf::Mouse::getPosition(Window).x;
      viewCenter.y = sf::Mouse::getPosition(Window).y;
    }
    // level.World.Step(1 / 60.f, 8, 3);

    Window.clear(sf::Color(43, 66, 93));

    // sf::Vector2f playerPos(player->getBody()->GetPosition().x * SCALE,
    // player->getBody()->GetPosition().y * SCALE);

    view.setCenter(viewCenter);
    Window.setView(view);

    // viewCenter += (playerPos - viewCenter) * 0.3f;

    Level &currentLevel = player1->getLevel();
    currentLevel.render(Window, viewCenter);
    Window.display();

    //FPS:
    // std::cout << elapsed.asMilliseconds() << std::endl;
  }

  return 0;
}

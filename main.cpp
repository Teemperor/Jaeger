#include <Character.h>
#include <Logger.h>
#include <PathFinder.h>
#include <SFML/Graphics.hpp>
#include <controls/GameUI.h>
#include <controls/PlayerControls.h>
#include <fstream>
#include <gamedata/GameData.h>
#include <iostream>
#include <level/Level.h>
#include <level/WorldGen.h>
#include <stdlib.h>
#include <unordered_map>

int main() {
  Logger mainLogger;

  srand(2);

  /** Prepare the window */
  assert(!sf::VideoMode::getFullscreenModes().empty());
  const sf::VideoMode &Mode = sf::VideoMode::getFullscreenModes().front();
  sf::RenderWindow Window(Mode, "Test", sf::Style::Default);

  Window.setFramerateLimit(60);

  sf::View view = Window.getView();
  view.zoom(0.3f);

  GameData Data(".");

  WorldGen gen;
  World *world = gen.generate(Data);

  Level &level = *world->getLevels().at(0);

  Faction playerFaction(true);

  Character *player1 = new Character(level, Vec2(60 * 16, 170 * 16),
                                     CharacterAI::Behavior::Bandit);
  player1->setFaction(&playerFaction);
  player1->addItem("wood_bow");

  Character *player2 = new Character(level, Vec2(60 * 16, 171 * 16),
                                     CharacterAI::Behavior::Bandit);
  player2->setFaction(&playerFaction);

  for (int i = 0; i < 0; i++) {
    Vec2 pos = Vec2(200 + rand() % 700, 200 + rand() % 700);
    if (level.passable(pos))
      new Character(level, Vec2(TilePos(pos)), CharacterAI::Behavior::Bandit);
    else {
      --i;
      continue;
    }
  }

  sf::Vector2f viewCenter;

  GameUI gui(Data, 1);
  player1->setPlayerControls(&gui.getControls());

  GameUI gui2(Data, 2);
  player2->setPlayerControls(&gui2.getControls());

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
      gui2.handleEvent(event);
      gui.handleEvent(event);
    }
    viewCenter.x = player1->getPos().getX();
    viewCenter.y = player1->getPos().getY();

    sf::Time elapsed = clock.restart();
    world->update(elapsed.asSeconds());

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      viewCenter.x = sf::Mouse::getPosition(Window).x;
      viewCenter.y = sf::Mouse::getPosition(Window).y;
    }
    // level.World.Step(1 / 60.f, 8, 3);

    Window.clear(sf::Color(22, 33, 44));

    // sf::Vector2f playerPos(player->getBody()->GetPosition().x * SCALE,
    // player->getBody()->GetPosition().y * SCALE);

    view.setCenter(viewCenter);
    Window.setView(view);

    // viewCenter += (playerPos - viewCenter) * 0.3f;

    Level &currentLevel = player1->getLevel();
    currentLevel.render(Window, viewCenter);

    gui2.draw(Window, elapsed.asSeconds());
    gui.draw(Window, elapsed.asSeconds());

    Window.display();
    // FPS:
    // std::cout << elapsed.asMilliseconds() << std::endl;
  }

  return 0;
}

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <vector>
#include <SFML/Window.hpp>

class PlayerControls {
public:

    void update(std::vector<sf::Event> events) {
/*
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
            == sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
          player->MoveDir = Character::MoveDirection::NONE;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
          player->MoveDir = Character::MoveDirection::RIGHT;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
          player->MoveDir = Character::MoveDirection::LEFT;
        }
        player->setFallThrough(sf::Keyboard::isKeyPressed(sf::Keyboard::S));

        for (sf::Event& event : events) {

            if (event.type == sf::Event::EventType::KeyPressed) {
              if (event.key.code == sf::Keyboard::LShift)
                player->jump();
              if (event.key.code == sf::Keyboard::J)
                player->shoot();
            }
        } */
    }
};

#endif // PLAYERCONTROLS_H

#ifndef CONTROLABLE_H
#define CONTROLABLE_H

#include <controls/PlayerControls.h>

class Controlable
{
  PlayerControls* controls = nullptr;
public:
  Controlable();
  virtual ~Controlable();
  void setPlayerControls(PlayerControls* c) {
    controls = c;
  }
  bool isControlled() {
    return controls != nullptr;
  }
  float getXInput() {
    return controls->getXInput();
  }
  float getYInput() {
    return controls->getYInput();
  }
};

#endif // CONTROLABLE_H

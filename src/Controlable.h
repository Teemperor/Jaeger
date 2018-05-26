#ifndef CONTROLABLE_H
#define CONTROLABLE_H

#include <cassert>

#include <controls/PlayerControls.h>

class Controlable {
  PlayerControls *Controls = nullptr;

public:
  Controlable();
  virtual ~Controlable();
  virtual void setPlayerControls(PlayerControls *C) { Controls = C; }
  bool isControlled() { return Controls != nullptr; }
  float getXInput() { return Controls->getXInput(); }
  float getYInput() { return Controls->getYInput(); }

  PlayerControls *getControls() {
    assert(Controls);
    return Controls;
  }
};

#endif // CONTROLABLE_H

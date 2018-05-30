#ifndef MAMBO_STATUSBAR_H
#define MAMBO_STATUSBAR_H


#include <SFML/Graphics.hpp>
#include <gamedata/GameData.h>

class StatusBar {
  int Length;
  sf::Sprite BarStart, BarMid, BarEnd;
  int Max = 100;
  int Value = 100;
  int OffX = 0;
  int OffY = 0;
public:
  StatusBar() = default;
  StatusBar(GameData &Data, const std::string &SpriteNamePrefix, int Length)
      : Length(Length) {

    BarStart = Data.getSprite(SpriteNamePrefix + "_start");
    BarMid = Data.getSprite(SpriteNamePrefix + "_mid");
    BarEnd = Data.getSprite(SpriteNamePrefix + "_end");
  }

  void setOffset(int X, int Y) {
    OffX = X;
    OffY = Y;
  }

  void setMax(int M) {
    Max = M;
  }

  void setValue(int V) {
    Value = V;
  }

  void draw(sf::RenderTarget &Target) {
    int Barlength = (int)(Value / (float) Max * Length);

    sf::Sprite S = BarStart;
    int Pos = 0;
    for (int I = 0; I < Barlength; ++I) {
      if (I == Barlength - 1)
        S = BarEnd;
      S.setPosition({(float)Pos + OffX, (float)OffY});
      Target.draw(S);
      Pos += S.getLocalBounds().width;
      S = BarMid;
    }
  }
};


#endif //MAMBO_STATUSBAR_H

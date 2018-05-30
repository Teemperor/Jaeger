#include "StatusBar.h"

void StatusBar::draw(sf::RenderTarget &Target, float dtime) {
  const float ShadowValueSpeed = ShadowValueSpeedPercentage * Max;

  float ShadowValueChange = dtime * ShadowValueSpeed;

  float ShadowDiff = Value - ShadowValue;
  if (std::abs(ShadowDiff) < ShadowValueChange) {
    ShadowValue = Value;
  } else {
    float ShadowDirection = Value < ShadowValue ? -1 : 1;
    ShadowValue += ShadowValueChange * ShadowDirection;
  }

  int Barlength = (int)(ShadowValue / (float) Max * Length);
  sf::Sprite S;
  int Pos = 0;

  S = BarStart;
  for (int I = 0; I < Barlength; ++I) {
    if (I == Barlength - 1)
      S = BarEnd;
    S.setColor(sf::Color(255, 0, 0, 180));
    S.setPosition({(float)Pos + OffX, (float)OffY});
    Target.draw(S);
    Pos += S.getLocalBounds().width;
    S = BarMid;
  }

  Barlength = (int)(Value / (float) Max * Length);
  S = BarStart;
  Pos = 0;
  for (int I = 0; I < Barlength; ++I) {
    if (I == Barlength - 1)
      S = BarEnd;
    S.setColor(sf::Color(255, 255, 255, 255));
    S.setPosition({(float)Pos + OffX, (float)OffY});
    Target.draw(S);
    Pos += S.getLocalBounds().width;
    S = BarMid;
  }

}

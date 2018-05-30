#include "StatusBar.h"

void StatusBar::draw(sf::RenderTarget &Target, float dtime) {
  const float ShadowValueSpeed = ShadowValueSpeedPercentage * Max;

  float MaxShadowValueChange = dtime * ShadowValueSpeed;

  

  ShadowValue += static_cast<float>((Value - ShadowValue) * 0.32 * dtime);

  std::cout << ShadowValue << std::endl;
  int Barlength = (int)(ShadowValue / (float) Max * Length);
  sf::Sprite S;
  int Pos = 0;

  S = BarStart;
  for (int I = 0; I < Barlength; ++I) {
    if (I == Barlength - 1)
      S = BarEnd;
    S.setColor(sf::Color(255, 255, 255, 100));
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

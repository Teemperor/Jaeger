#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H


class Attribute {

  int Value = 0;
  int Modifier = 0;
  int Max = 0;

public:
  Attribute(int Value, int Max);

  void damage(int v) {
    Modifier -= v;
  }

  void restore(int v) {
    if (Modifier < 0) {
      Modifier += v;
      if (Modifier > 0)
        Modifier = 0;
    }
  }

  void setValue(int v) {
    Value = v;
  }

  bool isBoosted() const {
    return Modifier > 0;
  }
  bool isDamaged() const {
    return Modifier < 0;
  }

  int getValue() const {
    int v = Value + Modifier;
    if (v > 0) {
      if (v > Max)
        return Max;
      return v;
    }
    return 0;
  }

};

#endif // ATTRIBUTE_H

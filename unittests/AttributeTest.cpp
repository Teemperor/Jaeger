#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"

#include "Attribute.h"

TEST_CASE("Damaging and restoring", "[Attribute]") {

  Attribute a(50, 100);

  REQUIRE(a.getValue() == 50);
  REQUIRE(!a.isBoosted());
  REQUIRE(!a.isDamaged());

  SECTION("damaging and restoring partly") {
    a.damage(10);
    REQUIRE(!a.isBoosted());
    REQUIRE(a.isDamaged());
    REQUIRE(a.getValue() == 40);
    a.restore(5);
    REQUIRE(!a.isBoosted());
    REQUIRE(a.isDamaged());
    REQUIRE(a.getValue() == 45);
  }
  SECTION("damaging and restoring fully") {
    a.damage(10);
    REQUIRE(!a.isBoosted());
    REQUIRE(a.isDamaged());
    REQUIRE(a.getValue() == 40);
    a.restore(10);
    REQUIRE(!a.isBoosted());
    REQUIRE(!a.isDamaged());
    REQUIRE(a.getValue() == 50);
  }
  SECTION("damaging and restoring more than fully") {
    a.damage(10);
    REQUIRE(!a.isBoosted());
    REQUIRE(a.isDamaged());
    REQUIRE(a.getValue() == 40);
    a.restore(15);
    REQUIRE(!a.isBoosted());
    REQUIRE(!a.isDamaged());
    REQUIRE(a.getValue() == 50);
  }
}

TEST_CASE("setValue", "[Attribute]") {
  Attribute a(50, 100);

  REQUIRE(a.getValue() == 50);
  a.setValue(45);
  REQUIRE(a.getValue() == 45);
  a.setValue(0);
  REQUIRE(a.getValue() == 0);
}

TEST_CASE("max min", "[Attribute]") {
  Attribute a(50, 100);

  REQUIRE(a.getValue() == 50);
  a.setValue(101);
  REQUIRE(a.getValue() == 100);
}

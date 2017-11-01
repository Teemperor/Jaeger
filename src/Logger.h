#ifndef ARGOS_LOGGER_H
#define ARGOS_LOGGER_H

#include <iostream>

class Logger {
  bool newLog = true;

public:
  Logger() {}

  Logger &operator<<(const std::string &s) {
    if (newLog) {
      newLog = false;
      std::cout << "[LOG] ";
    }
    std::cout << s;
    return *this;
  }

  void operator<<(Logger &) {
    newLog = true;
    std::cout << std::endl;
  }
};

#endif // ARGOS_LOGGER_H

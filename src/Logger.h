#ifndef ARGOS_LOGGER_H
#define ARGOS_LOGGER_H

#include <iostream>

class Logger {
  bool NewLog = true;

public:
  Logger() {}

  Logger &operator<<(const std::string &s) {
    if (NewLog) {
      NewLog = false;
      std::cout << "[LOG] ";
    }
    std::cout << s;
    return *this;
  }

  void operator<<(Logger &) {
    NewLog = true;
    std::cout << std::endl;
  }
};

#endif // ARGOS_LOGGER_H

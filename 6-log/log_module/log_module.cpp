#include <iostream>

#include "log.h"

int main() {
  umutech::log::initialize();
  _LOG_INFO() << "Hello World!";
  umutech::log::flush();
}

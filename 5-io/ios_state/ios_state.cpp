#include <iostream>

#include <boost/io/ios_state.hpp>

void PrintHex(std::ostream& os, char byte) {
  // Try commenting out the next line
  boost::io::ios_flags_saver ifs(os);

  std::cout << byte << " = ";
  os << std::hex << static_cast<unsigned>(byte);
  std::cout << '\n';
}

int main() {
  PrintHex(std::cout, 'A');
  std::cout << 123 << '\n';
  PrintHex(std::cerr, 'b');
  std::cout << 456 << '\n';
  PrintHex(std::cerr, 'C');
  std::cout << 789 << '\n';
}
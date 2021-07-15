#include <iostream>
#include <string>

#include <boost/io/quoted.hpp>

int main() {
  std::string buffer;
  std::getline(std::cin, buffer);
  std::cout << boost::io::quoted(buffer, '\\', '"') << '\n';
}

/*
* Input: C:\Program Files
* Output: "C:\\Program Files"
* 
* Input: {"name":"UMU618","male":true}
* Output: "{\"name\":\"UMU618\",\"male\":true}"
*/
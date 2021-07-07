// clang -std=c++20 test.cpp -ldl -lstdc++ -lstdc++fs -lboost_program_options
#include <filesystem>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  try {
    std::filesystem::path me(argv[0]);
    po::options_description desc(std::string("Usage of ") + me.filename().string());
    desc.add_options()("help,h", "produce help message");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc;
      return EXIT_SUCCESS;
    }
  } catch (std::exception& e) {
    std::cerr << "Invalid argument: " << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Invalid argument: unknown exception!\n";
    return EXIT_FAILURE;
  }

  std::cout << "OK!\n";
  return EXIT_SUCCESS;
}

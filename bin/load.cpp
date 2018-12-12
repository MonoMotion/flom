#include <fstream>
#include <stdexcept>

#include "flom/flom.hpp"

int main(int argc, char* argv[]) {
  std::ifstream f(argv[1]);
  auto const motion = flom::import_legacy_json(f);

  return 0;
}

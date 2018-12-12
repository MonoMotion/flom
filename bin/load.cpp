#include <fstream>
#include <iomanip>

#include "flom/flom.hpp"

int main(int argc, char* argv[]) {
  std::ifstream f(argv[1], std::ios::binary);
  auto const motion = flom::import_legacy_json(f);
  std::ofstream o(argv[2], std::ios::trunc | std::ios::binary);
  return motion.SerializeToOstream(&o);
}

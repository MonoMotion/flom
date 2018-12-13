#include <fstream>
#include <iomanip>

#include "flom/flom.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " INPUT OUTPUT" << std::endl;
    return -1;
  }

  std::ifstream f(argv[1], std::ios::binary);
  auto const motion = flom::Motion::load_legacy_json(f);
  std::ofstream o(argv[2], std::ios::trunc | std::ios::binary);
  motion.dump(o);
  return 0;
}

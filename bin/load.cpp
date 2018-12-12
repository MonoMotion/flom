#include "flom/flom.hpp"

#include "nlohmann/json.hpp"

int main(int argc, char* argv[]) {
  auto const motion = flom::load();
  return 0;
}

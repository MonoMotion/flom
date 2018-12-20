//
// Copyright 2018 coord.e
//
// This file is part of Flom.
//
// Flom is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Flom is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Flom.  If not, see <http://www.gnu.org/licenses/>.
//

#include <fstream>
#include <iomanip>
#include <iostream>

#include "flom/flom.hpp"

int main(int argc, char *argv[]) {
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

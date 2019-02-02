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

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck.h>
#include <rapidcheck/boost_test.h>

#if __has_include(<filesystem>)
#include <filesystem>
namespace filesystem = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#else
#error Could not find filesystem header
#endif

#include <fstream>
#include <iomanip>

#include <flom/motion.hpp>

#include "comparison.hpp"
#include "generators.hpp"
#include "printers.hpp"

BOOST_AUTO_TEST_SUITE(motion_io)

RC_BOOST_PROP(dump_load, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());

  auto const path = filesystem::temp_directory_path() / "out.fom";

  {
    std::ofstream f(path, std::ios::binary);

    // no throws
    m.dump(f);
  }

  {
    std::ifstream f(path, std::ios::binary);

    // no throws
    auto const m2 = m.load(f);
    filesystem::remove(path);

    FLOM_ALMOST_EQUAL(m, m2);
  }
}

RC_BOOST_PROP(dump_load_json, (const flom::Motion &m)) {
  RC_ASSERT(m.is_valid());

  auto const path = filesystem::temp_directory_path() / "out.json";

  {
    std::ofstream f(path);

    // no throws
    m.dump_json(f);
  }

  {
    std::ifstream f(path);

    // no throws
    auto const m2 = m.load_json(f);
    filesystem::remove(path);

    FLOM_ALMOST_EQUAL(m, m2);
  }
}

BOOST_AUTO_TEST_SUITE_END()

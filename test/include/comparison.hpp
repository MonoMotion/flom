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

#ifndef FLOM_TEST_OPERATORS_HPP
#define FLOM_TEST_OPERATORS_HPP

#include <flom/effector.hpp>
#include <flom/effector_weight.hpp>
#include <flom/frame.hpp>
#include <flom/motion.hpp>

#include <rapidcheck.h>

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>

// loose comparison functions to use in tests.

namespace flom {

namespace testing {

bool almost_equal(double, double);

bool almost_equal(const qvm::vec<double, 3> &, const qvm::vec<double, 3> &);
bool almost_equal(const qvm::quat<double> &, const qvm::quat<double> &);

bool almost_equal(const Location &, const Location &);
bool almost_equal(const Rotation &, const Rotation &);

bool almost_equal(const Effector &, const Effector &);
bool almost_equal(const Frame &, const Frame &);

bool almost_equal(const EffectorDifference &, const EffectorDifference &);
bool almost_equal(const FrameDifference &, const FrameDifference &);

bool almost_equal(const EffectorWeight &, const EffectorWeight &);
bool almost_equal(const Motion &, const Motion &);

} // namespace testing

} // namespace flom

#define FLOM_ALMOST_EQUAL(a, b) RC_ASSERT(flom::testing::almost_equal(a, b))

#endif

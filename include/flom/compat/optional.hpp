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

#ifndef FLOM_COMPAT_OPTIONAL_HPP
#define FLOM_COMPAT_OPTIONAL_HPP


#if __has_include(<optional>)

#include <optional>

namespace flom::compat {
template<typename T>
using optional = std::optional<T>;
}

#elif __has_include(<experimental/optional>)

#include <experimental/optional>

namespace flom::compat {
template<typename T>
using optional = std::experimental::optional<T>;
}

#else
#error Could not find optional header
#endif

#endif

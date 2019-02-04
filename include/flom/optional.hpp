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

#ifndef FLOM_OPTIONAL_HPP
#define FLOM_OPTIONAL_HPP

#include "flom/compat/optional.hpp"

namespace flom::optional {

static inline auto nullopt = compat::nullopt;
using nullopt_t = decltype(nullopt);

//
// a class template for an optional contained value of T
//
// expected to work as if this is an alias of std::optional
//
// Background:
// the real signature of compat::optional changes
// depending on the language processor.
// If libflom.so is compiled with std::optional,
// that cannot be linked with libflom.so with boost::optional.
// With this class, signature is always `flom::optional::optional`
// and linking failure cannot be happen.
//
template <typename T> class optional : public compat::optional<T> {
public:
  using base_type = compat::optional<T>;

  using base_type::base_type;
  using base_type::operator=;
};

} // namespace flom::optional

#endif

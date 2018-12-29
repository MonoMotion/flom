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

#ifndef FLOM_ERRORS_HPP
#define FLOM_ERRORS_HPP

#include <stdexcept>

namespace flom::errors {

class InvalidTimeError : public std::runtime_error {
public:
  explicit InvalidTimeError(double);
  virtual const char *what() const noexcept;
};

class OutOfFramesError : public std::out_of_range {
public:
  explicit OutOfFramesError(double);
  virtual const char *what() const noexcept;
};

class ParseError : public std::runtime_error {
public:
  // TODO: include additional information
  ParseError();
  virtual const char *what() const noexcept;
};

class SerializationError : public std::runtime_error {
public:
  // TODO: include additional information
  SerializationError();
  virtual const char *what() const noexcept;
};

class JSONLoadError : public std::runtime_error {
public:
  explicit JSONLoadError(const std::string &);
  virtual const char *what() const noexcept;
};

class JSONDumpError : public std::runtime_error {
public:
  explicit JSONDumpError(const std::string &);
  virtual const char *what() const noexcept;
};

} // namespace flom::errors

#endif

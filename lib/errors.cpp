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

#include <flom/errors.hpp>

#include <string>

namespace flom::errors {

using namespace std::string_literals;

InvalidTimeError::InvalidTimeError(double t)
    : std::runtime_error("Time '" + std::to_string(t) + "' is invalid") {}

const char *InvalidTimeError::what() const noexcept {
  return ("InvalidTimeError: "s + std::runtime_error::what()).c_str();
}

OutOfFramesError::OutOfFramesError(double t)
    : std::out_of_range("No frame is avaliable at time " + std::to_string(t)) {}

const char *OutOfFramesError::what() const noexcept {
  return ("OutOfFramesError: "s + std::out_of_range::what()).c_str();
}

ParseError::ParseError() : std::runtime_error("Could not parse input") {}

const char *ParseError::what() const noexcept {
  return ("ParseError: "s + std::runtime_error::what()).c_str();
}

SerializationError::SerializationError()
    : std::runtime_error("Could not serialize data") {}

const char *SerializationError::what() const noexcept {
  return ("SerializationError: "s + std::runtime_error::what()).c_str();
}

JSONLoadError::JSONLoadError(const std::string &message)
    : std::runtime_error("Failed to load from JSON string: " + message) {}

const char *JSONLoadError::what() const noexcept {
  return ("JSONLoadError: "s + std::runtime_error::what()).c_str();
}

JSONDumpError::JSONDumpError(const std::string &message)
    : std::runtime_error("Failed to dump to JSON string: " + message) {}

const char *JSONDumpError::what() const noexcept {
  return ("JSONDumpError: "s + std::runtime_error::what()).c_str();
}

} // namespace flom::errors

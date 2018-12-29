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

BaseError::BaseError(const std::string &message_) noexcept
    : message(message_) {}

const char *BaseError::what() const noexcept { return this->message.c_str(); }

InvalidTimeError::InvalidTimeError(double t)
    : BaseError("Time '" + std::to_string(t) + "' is invalid") {}

const char *InvalidTimeError::what() const noexcept {
  return this->message.c_str();
}

OutOfFramesError::OutOfFramesError(double t)
    : BaseError("No frame is avaliable at time " + std::to_string(t)) {}

const char *OutOfFramesError::what() const noexcept {
  return this->message.c_str();
}

ParseError::ParseError() : BaseError("Could not parse input") {}

const char *ParseError::what() const noexcept { return this->message.c_str(); }

SerializationError::SerializationError()
    : BaseError("Could not serialize data") {}

const char *SerializationError::what() const noexcept {
  return this->message.c_str();
}

JSONLoadError::JSONLoadError(const std::string &message)
    : BaseError("Failed to load from JSON string: " + message) {}

const char *JSONLoadError::what() const noexcept {
  return this->message.c_str();
}

JSONDumpError::JSONDumpError(const std::string &message)
    : BaseError("Failed to dump to JSON string: " + message) {}

const char *JSONDumpError::what() const noexcept {
  return this->message.c_str();
}

} // namespace flom::errors

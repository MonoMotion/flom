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

InvalidTimeError::InvalidTimeError(double time) : t(time) {}

const char *InvalidTimeError::what() const noexcept {
  return "Invalid time is supplied";
}

double InvalidTimeError::time() const noexcept { return this->t; }

OutOfFramesError::OutOfFramesError(double time) : t(time) {}

const char *OutOfFramesError::what() const noexcept {
  return "No frame is available at supplied time";
}

double OutOfFramesError::time() const noexcept { return this->t; }

KeyframeNotFoundError::KeyframeNotFoundError(double time) : t(time) {}

const char *KeyframeNotFoundError::what() const noexcept {
  return "Keyframe not found at supplied time";
}

double KeyframeNotFoundError::time() const noexcept { return this->t; }

ParseError::ParseError() {}

const char *ParseError::what() const noexcept {
  return "Could not parse input";
}

SerializationError::SerializationError() {}

const char *SerializationError::what() const noexcept {
  return "Could not serialize data";
}

JSONLoadError::JSONLoadError(const std::string &message) : status(message) {}

const char *JSONLoadError::what() const noexcept {
  return "Failed to load from JSON string";
}

std::string JSONLoadError::status_message() const noexcept {
  return this->status;
}

JSONDumpError::JSONDumpError(const std::string &message) : status(message) {}

const char *JSONDumpError::what() const noexcept {
  return "Failed to dump to JSON string";
}

std::string JSONDumpError::status_message() const noexcept {
  return this->status;
}

InvalidFrameError::InvalidFrameError(const std::string &message)
    : status(message) {}

const char *InvalidFrameError::what() const noexcept {
  return "Invalid motion data is detected";
}

std::string InvalidFrameError::status_message() const noexcept {
  return this->status;
}

} // namespace flom::errors

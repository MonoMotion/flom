#include <flom/errors.hpp>

#include <string>

namespace flom::errors {

InvalidTimeError::InvalidTimeError(double t)
    : std::runtime_error("Time '" + std::to_string(t) + "' is invalid") {}

OutOfFramesError::OutOfFramesError(double t)
    : std::out_of_range("No frame is avaliable at time " + std::to_string(t)) {}

ParseFailedError::ParseFailedError()
    : std::runtime_error("Could not parse input") {}

SerializationFailedError::SerializationFailedError()
    : std::runtime_error("Could not serialize data") {}

JSONLoadError::JSONLoadError(const std::string &message)
    : std::runtime_error("Failed to load from JSON string: " + message) {}

JSONDumpError::JSONDumpError(const std::string &message)
    : std::runtime_error("Failed to dump to JSON string: " + message) {}

} // namespace flom::errors

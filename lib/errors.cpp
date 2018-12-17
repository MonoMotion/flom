#include <flom/errors.hpp>

#include <string>

namespace flom::errors {

InvalidTimeError::InvalidTimeError(double t)
    : std::runtime_error("Time '" + std::to_string(t) + "' is invalid") {}

OutOfFramesError::OutOfFramesError(double t)
    : std::out_of_range("No frame is avaliable at time " + std::to_string(t)) {}

} // namespace flom::errors

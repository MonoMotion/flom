#ifndef FLOM_ERRORS_HPP
#define FLOM_ERRORS_HPP

#include <stdexcept>

namespace flom::errors {

class InvalidTimeError : public std::runtime_error {
public:
  explicit InvalidTimeError(double);
};

class OutOfFramesError : public std::out_of_range {
public:
  explicit OutOfFramesError(double);
};

class ParseFailedError : public std::runtime_error {
public:
  // TODO: include additional information
  ParseFailedError();
};

} // namespace flom::errors

#endif

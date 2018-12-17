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

class ParseError : public std::runtime_error {
public:
  // TODO: include additional information
  ParseError();
};

class SerializationError : public std::runtime_error {
public:
  // TODO: include additional information
  SerializationError();
};

class JSONLoadError : public std::runtime_error {
public:
  explicit JSONLoadError(const std::string &);
};

class JSONDumpError : public std::runtime_error {
public:
  explicit JSONDumpError(const std::string &);
};

} // namespace flom::errors

#endif

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

#ifndef FLOM_ERRORS_HPP
#define FLOM_ERRORS_HPP

#include <stdexcept>

namespace flom::errors {

class BaseError : public std::exception {
public:
  explicit BaseError(const std::string &) noexcept;
  virtual const char *what() const noexcept;

protected:
  std::string message;
};

class InvalidTimeError : public BaseError {
public:
  explicit InvalidTimeError(double);
  virtual const char *what() const noexcept;
};

class OutOfFramesError : public BaseError {
public:
  explicit OutOfFramesError(double);
  virtual const char *what() const noexcept;
};

class ParseError : public BaseError {
public:
  // TODO: include additional information
  ParseError();
  virtual const char *what() const noexcept;
};

class SerializationError : public BaseError {
public:
  // TODO: include additional information
  SerializationError();
  virtual const char *what() const noexcept;
};

class JSONLoadError : public BaseError {
public:
  explicit JSONLoadError(const std::string &);
  virtual const char *what() const noexcept;
};

class JSONDumpError : public BaseError {
public:
  explicit JSONDumpError(const std::string &);
  virtual const char *what() const noexcept;
};

} // namespace flom::errors

#endif

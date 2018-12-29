#ifndef FLOM_ERRORS_HPP
#define FLOM_ERRORS_HPP

#include <stdexcept>

namespace flom::errors {

class InvalidTimeError : public std::exception {
public:
  explicit InvalidTimeError(double);
  virtual const char *what() const noexcept;

  double time() const noexcept;

private:
  double t;
};

class OutOfFramesError : public std::exception {
public:
  explicit OutOfFramesError(double);
  virtual const char *what() const noexcept;

  double time() const noexcept;

private:
  double t;
};

class ParseError : public std::exception {
public:
  // TODO: include additional information
  ParseError();
  virtual const char *what() const noexcept;
};

class SerializationError : public std::exception {
public:
  // TODO: include additional information
  SerializationError();
  virtual const char *what() const noexcept;
};

class JSONLoadError : public std::exception {
public:
  explicit JSONLoadError(const std::string &);
  virtual const char *what() const noexcept;

  std::string status_message() const noexcept;

public:
  std::string status;
};

class JSONDumpError : public std::exception {
public:
  explicit JSONDumpError(const std::string &);
  virtual const char *what() const noexcept;

  std::string status_message() const noexcept;

public:
  std::string status;
};

} // namespace flom::errors

#endif

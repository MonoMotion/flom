#ifndef FLOM_MOTION_HPP
#define FLOM_MOTION_HPP

#include "flom/frame.hpp"

#include <string>
#include <fstream>
#include <memory>
#include <unordered_map>

namespace flom {

enum class LoopType {
  None,
  Wrap
};

struct FrameRange;

class Motion {
public:
  static Motion load(std::ifstream&);
  static Motion load_json(std::ifstream&);
  static Motion load_json_string(std::string const&);
  static Motion load_legacy_json(std::ifstream&);

  Motion();
  // With model name
  Motion(std::string const&);

  Motion(Motion const&);
  ~Motion();

  Frame frame_at(double t) const;

  FrameRange frames(double fps) const;

  bool is_in_range_at(double t) const;

  void dump(std::ofstream&) const;
  void dump_json(std::ofstream&) const;
  std::string dump_json_string() const;

  LoopType loop() const;
  void set_loop(LoopType);

  std::string model_id() const;
  void set_model_id(std::string const&);

  Frame& get_or_insert_frame(double t);

private:
  class Impl;
  std::unique_ptr<Impl> impl;
};

}

#endif

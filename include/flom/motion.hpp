#ifndef FLOM_MOTION_HPP
#define FLOM_MOTION_HPP

#include "flom/frame.hpp"
#include "motion.pb.h"

#include <string>
#include <map>
#include <unordered_map>
#include <fstream>

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

  Motion(Motion const&);
  ~Motion();

  Frame frame_at(double t) const;

  FrameRange frames(double fps) const;

  bool is_in_range_at(double t) const;

  void dump(std::ofstream&) const;
  void dump_json(std::ofstream&) const;
  std::string dump_json_string() const;

private:
  Motion();

  class Impl;
  std::unique_ptr<Impl> impl;
};

}

#endif

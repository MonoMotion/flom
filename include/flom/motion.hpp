#ifndef FLOM_MOTION_HPP
#define FLOM_MOTION_HPP

#include "flom/frame.hpp"
#include "motion.pb.h"

#include <string>
#include <map>
#include <fstream>

namespace flom {

enum class LoopType {
  None,
  Wrap
};

class Motion {
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> frames;

public:
  static Motion load(std::ifstream&);

  Frame const& frame_at(double t) const;

  void dump(std::ofstream&) const;

private:
  static Motion from_protobuf(proto::Motion const&);
  proto::Motion to_protobuf() const;
};

}

#endif

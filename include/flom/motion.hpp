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

  Frame frame_at(double t) const;

  void dump(std::ofstream&) const;

private:
  static Motion from_protobuf(proto::Motion const&);
  proto::Motion to_protobuf() const;
};

namespace proto_util {

boost::qvm::vec<double, 3> convert_vec3(proto::Vec3 const&);
Translation convert_translation(proto::Translation const&);

boost::qvm::quat<double> convert_quat(proto::Quaternion const&);
Rotation convert_rotation(proto::Rotation const&);

}

}

#endif

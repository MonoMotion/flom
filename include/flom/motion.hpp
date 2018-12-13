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

void pack_vec3(boost::qvm::vec<double, 3> const&, proto::Vec3*);
void pack_translation(Translation const&, proto::Translation*);

boost::qvm::vec<double, 3> unpack_vec3(proto::Vec3 const&);
Translation unpack_translation(proto::Translation const&);

void pack_quat(boost::qvm::quat<double> const&, proto::Quaternion*);
void pack_rotation(Rotation const&, proto::Rotation*);

boost::qvm::quat<double> unpack_quat(proto::Quaternion const&);
Rotation unpack_rotation(proto::Rotation const&);

}

}

#endif

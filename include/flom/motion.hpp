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
void convert_vec3(boost::qvm::vec<double, 3> const& vec, proto::Vec3* v_proto);
Translation convert_translation(proto::Translation const&);
void convert_translation(Translation const&, proto::Translation*);

boost::qvm::quat<double> convert_quat(proto::Quaternion const&);
void convert_quat(boost::qvm::quat<double> const& quat, proto::Quaternion* q_proto);
Rotation convert_rotation(proto::Rotation const&);
void convert_rotation(Rotation const&, proto::Rotation*);

}

}

#endif

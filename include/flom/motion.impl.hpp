#ifndef FLOM_MOTION_IMPL_HPP
#define FLOM_MOTION_IMPL_HPP

#include "flom/frame.hpp"
#include "flom/motion.hpp"

#include "motion.pb.h"

#include <map>
#include <string>

namespace flom {

class Motion::Impl {
public:
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> raw_frames;
  std::unordered_map<std::string, EffectorType> effector_types;

  Impl() : model_id(), loop(LoopType::None), raw_frames(), effector_types() {}
  Impl(std::string const &model)
      : model_id(model), loop(LoopType::None), raw_frames(), effector_types() {}

  static Motion from_protobuf(proto::Motion const &);
  proto::Motion to_protobuf() const;

  bool is_valid() const;
};

} // namespace flom

#endif

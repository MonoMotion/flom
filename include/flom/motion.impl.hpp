#ifndef FLOM_MOTION_IMPL_HPP
#define FLOM_MOTION_IMPL_HPP

#include "flom/motion.hpp"
#include "flom/frame.hpp"

#include "motion.pb.h"

#include <string>
#include <map>

namespace flom {

class Motion::Impl {
public:
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> raw_frames;

  Impl()
    : model_id(), loop(LoopType::None), raw_frames() {}
  Impl(std::string const& model)
    : model_id(model), loop(LoopType::None), raw_frames() {}

  static Motion from_protobuf(proto::Motion const&);
  proto::Motion to_protobuf() const;
};

}

#endif

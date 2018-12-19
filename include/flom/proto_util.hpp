#ifndef FLOM_PROTO_UTIL_HPP
#define FLOM_PROTO_UTIL_HPP

#include "flom/effector.hpp"
#include "flom/motion.hpp"

#include "frame.pb.h"
#include "location.pb.h"
#include "motion.pb.h"
#include "rotation.pb.h"

#include <boost/qvm/quat.hpp>
#include <boost/qvm/vec.hpp>

namespace flom::proto_util {

void pack_vec3(boost::qvm::vec<double, 3> const &, proto::Vec3 *);
void pack_location(Location const &, proto::Location *);

boost::qvm::vec<double, 3> unpack_vec3(proto::Vec3 const &);
Location unpack_location(proto::Location const &);

void pack_quat(boost::qvm::quat<double> const &, proto::Quaternion *);
void pack_rotation(Rotation const &, proto::Rotation *);

boost::qvm::quat<double> unpack_quat(proto::Quaternion const &);
Rotation unpack_rotation(proto::Rotation const &);

void pack_effector_type(EffectorType const &, proto::EffectorType *);
proto::EffectorType::Type pack_coord_system(CoordinateSystem const &);

EffectorType unpack_effector_type(proto::EffectorType const &);
CoordinateSystem unpack_coord_system(proto::EffectorType::Type const &);

} // namespace flom::proto_util

#endif

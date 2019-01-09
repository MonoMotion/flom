#include "flom/effector_type.hpp"

namespace flom {

EffectorType::EffectorType(std::optional<CoordinateSystem> location,
                           std::optional<CoordinateSystem> rotation)
    : location_(location), rotation_(rotation) {}

std::optional<CoordinateSystem> EffectorType::location() const {
  return this->location_;
}
std::optional<CoordinateSystem> EffectorType::rotation() const {
  return this->rotation_;
}

void EffectorType::set_location(std::optional<CoordinateSystem> location) {
  this->location_ = location;
}
void EffectorType::set_rotation(std::optional<CoordinateSystem> rotation) {
  this->rotation_ = rotation;
}

} // namespace flom

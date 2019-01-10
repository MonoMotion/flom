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

Effector EffectorType::new_effector() const {
  Effector e;
  if (this->location()) {
    e.location = Location{};
  }
  if (this->rotation()) {
    e.rotation = Rotation{};
  }
  return e;
}

bool EffectorType::is_compatible(const Effector &e) const {
  bool loc_v = static_cast<bool>(e.location);
  bool loc_t = static_cast<bool>(this->location());
  bool rot_v = static_cast<bool>(e.rotation);
  bool rot_t = static_cast<bool>(this->rotation());
  return loc_v == loc_t && rot_v == rot_t;
}

} // namespace flom

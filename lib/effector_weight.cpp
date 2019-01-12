#include "flom/effector_weight.hpp"
#include "flom/errors.hpp"

#include <cmath>

namespace flom {

EffectorWeight::EffectorWeight(double location, double rotation)
    : location_(validate_weight(location)),
      rotation_(validate_weight(rotation)) {}

double EffectorWeight::location() const noexcept { return this->location_; }
double EffectorWeight::rotation() const noexcept { return this->rotation_; }

void EffectorWeight::set_location(double weight) {
  this->location_ = EffectorWeight::validate_weight(weight);
}

void EffectorWeight::set_rotation(double weight) {
  this->rotation_ = EffectorWeight::validate_weight(weight);
}

double EffectorWeight::validate_weight(double weight) {
  if (1 < weight || weight < 0 || std::isnan(weight)) {
    throw errors::InvalidWeightError{weight};
  }
  return weight;
}

bool operator==(const EffectorWeight &v1, const EffectorWeight &v2) {
  return v1.location() == v2.location() && v1.rotation() == v2.rotation();
}

} // namespace flom

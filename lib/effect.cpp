#include "flom/effect.hpp"

#include <utility>

#include <boost/qvm/quat_operations.hpp>

namespace flom {

Effect interpolate(double t, Effect const& a, Effect const& b) {
  Effect e;
  if (a.rotation && b.rotation) {
    e.rotation = qvm::slerp(a.rotation->quat, b.rotation->quat, t);
  }
  if (a.translation && b.translation) {
    e.translation = a.translation->vec + t * (b.translation->vec - a.translation->vec);
  }
  return std::move(e);
}

}



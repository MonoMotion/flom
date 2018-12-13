#ifndef FLOM_FLOM_HPP
#define FLOM_FLOM_HPP

#include "flom/motion.hpp"
#include "flom/frame.hpp"
#include "flom/effect.hpp"

namespace flom {

proto::Motion import_legacy_json(std::ifstream &f);

}

#endif

#ifndef FLOM_FLOM_HPP
#define FLOM_FLOM_HPP

#include "motion.pb.h"

namespace flom {

proto::Motion import_legacy_json(std::ifstream &f);

}

#endif

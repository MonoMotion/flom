#ifndef FLOM_CONSTANTS_HPP
#define FLOM_CONSTANTS_HPP

namespace flom::constants {

template<typename T>
static constexpr auto pi = static_cast<T>(3.1415926535897932384626);

static constexpr int compare_ulp = 2;

}

#endif

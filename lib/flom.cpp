#include <fstream>
#include <stdexcept>

#include "flom/flom.hpp"

#include "nlohmann/json.hpp"

namespace flom {

using json = nlohmann::json;

proto::Motion import_legacy_json(std::ifstream &f) {
  json json_data;
  f >> json_data;

  proto::Motion m;
  {
    auto loop_type = json_data["loop"];
    if(loop_type == "wrap") {
      m.set_loop(proto::Motion::Loop::Motion_Loop_Wrap);
    } else if(loop_type == "none") {
      m.set_loop(proto::Motion::Loop::Motion_Loop_None);
    } else {
      throw std::runtime_error("Unknown loop type");
    }
  }
  return m;
}

}

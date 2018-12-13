#ifndef FLOM_MOTION_HPP
#define FLOM_MOTION_HPP

namespace flom {

enum class LoopType {
  None,
  Wrap
};

class Motion {
  std::string model_id;
  LoopType loop;
  std::map<double, Frame> frames;

public:
  static Motion load(std::ifstream&);

  Frame const& frame_at(double t) const;

  void dump(std::ofstream&);
};

}

#endif

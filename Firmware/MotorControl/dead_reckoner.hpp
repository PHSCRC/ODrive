#ifndef __DEAD_RECKONER_HPP
#define __DEAD_RECKONER_HPP
class DeadReckoner{
  struct Config_t {
    bool leftAxisZero=false;

    float wheelRadius=0.045f;
    float wheelDistance=20.0f;
  };
  DeadReckoner(Config_t& config);
  Config_t& config;

  float dX;
  float dY;
  float dTheta;

  bool alreadyCalled=false;
  void cond_update();
  void do_update();
  void reset();

  auto make_protocol_definitions() {
    return make_protocol_member_list(
      make_protocol_property("dX", &dX),
      make_protocol_property("dY", &dY),
      make_protocol_property("dTheta", &dTheta),
      make_protocol_object("config",
        make_protocol_property("leftAxisZero", &config.leftAxisZero),
        make_protocol_property("wheelRadius", &config.wheelRadius),
        make_protocol_property("wheelDistance", &config.wheelDistance),
      )
    );
  }
};


#endif /* end of include guard: __DEAD_RECKONER_HPP */

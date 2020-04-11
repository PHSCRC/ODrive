#include "dead_reckoner.hpp"
void DeadReckoner::do_update(){
  float old_theta=dTheta;
  dTheta=config_.wheelRadius/config_.wheelDistance*(encoders[0].pos_estimate_-encoders[1].pos_estimate_);
  if(config_.leftAxisZero){
    dTheta=-dTheta;
  }
  float d_theta=dTheta-old_theta;
  float d_f=(config_.wheelRadius*(encoders[0].vel_estimate_+encoders[1].vel_estimate_)/2.0f) //forward velocity
  /
  ((config_.wheelRadius/config_.wheelDistance*(encoders[0].vel_estimate_-encoders[1].vel_estimate_)) //angular velocity
  *
  2.0f*fabsf(d_theta*0.5f)); //sqrt(sin^2(x)+(1-cos(x))^2)=2*abs(sin(x/2))
  dX+=d_f*our_arm_cos_f32(dTheta);
  dY+=d_f*our_arm_sin_f32(dTheta);
}

void DeadReckoner::cond_update(Axis * axisWhichCalled){
  static bool zeroCalled;
  static bool oneCalled;
  zeroCalled=(axes[0]==axisWhichCalled);
  oneCalled=(axes[1]==axisWhichCalled);
  //once both of them have called this function, that means that both have called their update() function, so we can proceed
  if(zeroCalled && oneCalled){
    doUpdate();
    zeroCalled=false;
    oneCalled=false;
  }
}
void DeadReckoner::reset(){
  dX=0;
  dY=0;
  dTheta=0;
}

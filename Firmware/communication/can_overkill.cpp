#include "can_overkill.hpp"
#include <odrive_main.h>

static void CANOverkill::send_dead_reckoning(DeadReckoner * dead_reck){
  //getting the latest timestamp
  odCAN->make_silent();
  uint32_t mailbox=odCAN->writeEmpty(TIMER_MSG_ID);
  HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY);
  can_Message_t txmsg;
  odom_odrive_struct* data=(odom_odrive_struct*)txmsg.data;
  txmsg.id=odom_odrive_struct::id;
  txmsg.isExt=false;
  txmsg.len=odom_odrive_struct::size;

  //could have been released already, but that's fine we just move on.
  osSemaphoreWait(sem_can, 5); //after 5ms, something has clearly gone wrong, just ignore it
  odCAN->make_noisy();
  //even if this is an overflow loop, it should give the correct answer
  //this goes up every 4us, so divide by 250 to get in ms
  uint16_t diff=(odCAN->lastTxMessageTimestamp(mailbox)-lastNCANTimerValue)/250
  if(diff>255){
    diff=255;
  }
  data->ts.n=latestN;
  data->ts.msSinceN=(uint8_t)diff;
  data->dX=static_cast<fixed_point<1, true>>(dead_reck->dX);
  data->dY=static_cast<fixed_point<1, true>>(dead_reck->dY);
  data->dTheta=static_cast<fixed_point<7, false>>(toStdPos(dead_reck->dTheta));
  dead_reck->reset();
  odCAN->write(txmsg);
}
static CANOverkill::cmd_vel_callback(cmd_vel_struct* data){
  float vX=static_cast<float>(data->vX);
  float vTheta=static_cast<float>(data->vTheta);
  float wheelDistance=dead_reck->config_->wheelDistance;
  float wheelRadius=dead_reck->config_->wheelRadius;
  float rightVelRadPerSec=(2.0f*vX+wheelDistance*vTheta)/(2.0f*wheelRadius);
  float leftVelRadPerSec=(2.0f*vX-wheelDistance*vTheta)/(2.0f*wheelRadius);
  uint8_t rightAxis=!dead_reck->config_->leftAxisZero ? 0 : 1;
  uint8_t leftAxis=dead_reck->config_->leftAxisZero ? 0 : 1;
  axis[left].controller_.set_vel_setpoint(leftVelRadPerSec*0.159154943f*axis[left].encoder_.config_.cpr)
  axis[right].controller_.set_vel_setpoint(rightVelRadPerSec*0.159154943f*axis[right].encoder_.config_.cpr)
}
static void CANOverkill::handle_can_message(can_Message_t& msg){
  switch(msg.id){
    case synchronised_time_struct::id:
      latestNCANTimerValue=msg.timestamp;
      latestN=msg.data[0];
    break;
    case cmd_vel_struct::id:
      cmd_vel_callback((cmd_vel_struct*)msg.data)
    break;
  }
}
static void CANOverkill::condSend_dead_reck(DeadReckoner * dead_reck){
  uint16_t capturedCount=htim14.Instance->CNT;

  //equivalent to the following truth table:
  /*        BC
      00  01  11  10
  0  | 0 | x | 0 | 1 |
A    -----------------
  1  | 1 | 0 | 1 | x |
  */
/*                 A                              C                         B                 */
  if((targetCount>lastSentCount)^(capturedCount>lastSentCount)^(capturedCount>=targetCount)){
    targetCount=(targetCount+DESIREDTIM14PRESCALEHZ/DEAD_RECK_HZ)%TIM_APB1_PERIOD_CLOCKS;
    lastSentCount=capturedCount;
    send_dead_reckoning(dead_reck);
  }

}

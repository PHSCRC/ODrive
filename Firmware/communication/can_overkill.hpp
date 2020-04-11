#ifndef _CAN_OVERKILL_HPP_
#define _CAN_OVERKILL_HPP_

#include "interface_can.hpp"
#include "IAndOStructs.hpp"

#define DEAD_RECK_HZ 20

class CANOverkill{
  static uint16_t lastSentCount=0;
  static uint16_t targetCount=1

  //internal can timer overflows every 0.26 seconds at 250kbit
  //this means that when we can only miss one synchronised_timer can message before all our timestamps our messed up
  static uint16_t lastNCANTimerValue;

  static uint8_t latestN;

  static void send_dead_reckoning(DeadReckoner * dead_reck);
  static cmd_vel_callback(cmd_vel_struct* data);
  static void handle_can_message(can_Message_t& msg);
}

#endif /* end of include guard: _CAN_OVERKILL_HPP_ */

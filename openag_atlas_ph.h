/**
 *  \file openag_atlas_ph.h
 *  \brief Potential hydrogen sensor.
 */
#ifndef OPENAG_ATLAS_PH_H
#define OPENAG_ATLAS_PH_H

#include "Arduino.h"
#include <Wire.h>
#include <openag_module.h>
#include <std_msgs/Float32.h>

/**
 * \brief Potential hydrogen sensor.
 */
class AtlasPh : public Module {
  public:
    AtlasPh(int i2c_address);
    void begin();
    void update();
    bool get_water_potential_hydrogen(std_msgs::Float32 &msg);

  private:
    // Private variables
    float _water_potential_hydrogen;
    bool _send_water_potential_hydrogen;
    uint32_t _time_of_last_reading;
    uint32_t _time_of_last_query;
    bool _waiting_for_response;
    const static uint32_t _min_update_interval = 2000;
    int _i2c_address;

    // Private functions
    void send_query();
    void read_response();
};

#endif

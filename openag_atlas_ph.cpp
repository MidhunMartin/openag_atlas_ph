/**
 *  \file openag_atlas_ph.cpp
 *  \brief Potential hydrogen sensor.
 */
#include "openag_atlas_ph.h"

AtlasPh::AtlasPh(int i2c_address) {
    _i2c_address = i2c_address;
}

void AtlasPh::begin() {
  Wire.begin();
  _time_of_last_reading = 0;
  _time_of_last_query = 0;
  _waiting_for_response = false;
}

bool AtlasPh::get_water_potential_hydrogen(std_msgs::Float32 &msg) {
  if (_waiting_for_response) {
    if (millis() - _time_of_last_query > 1000) {
      _waiting_for_response = false;
      _time_of_last_reading = millis();
      if (read_response()) {
        msg.data = _water_potential_hydrogen;
        return true;
      }
    }
    return false;
  }
  else if (millis() - _time_of_last_reading > _min_update_interval){
    send_query();
  }
  return false;
}

void AtlasPh::send_query() {
  Wire.beginTransmission(_i2c_address);
  Wire.print("R");
  Wire.endTransmission();
  _waiting_for_response = true;
  _time_of_last_query = millis();
}

bool AtlasPh::read_response() {
  Wire.requestFrom(_i2c_address, 20, 1);
  byte response = Wire.read(); // increment buffer by a byte
  String string = Wire.readStringUntil(0);

  // Check for failure
  if (response == 255) {
    error_msg = "No data";
    has_error = true;
  }
  else if (response == 254) {
    error_msg = "Tried to read data before request was processed";
    has_error = true;
  }
  else if (response == 2) {
    error_msg = "Request failed";
    has_error = true;
  }
  else if (response == 1) { // good reading
    _water_potential_hydrogen = string.toFloat();
  }
  else {
    error_msg = "Unknown error";
    has_error = true;
  }
  return !has_error;
}

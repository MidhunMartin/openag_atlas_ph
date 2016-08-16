/**
 *  \file openag_atlas_ph.cpp
 *  \brief Potential hydrogen sensor.
 */
#include "openag_atlas_ph.h"

AtlasPh::AtlasPh(int i2c_address) {
  status_level = OK;
  status_msg = "";
  _send_water_potential_hydrogen = false;
  _time_of_last_reading = 0;
  _time_of_last_query = 0;
  _waiting_for_response = false;
  _i2c_address = i2c_address;
}

void AtlasPh::begin() {
  Wire.begin();
}

void AtlasPh::update() {
  if (_waiting_for_response) {
    if (millis() - _time_of_last_query > 1000) {
      _waiting_for_response = false;
      _time_of_last_reading = millis();
      read_response();
    }
  }
  else if (millis() - _time_of_last_reading > _min_update_interval) {
    send_query();
  }
}

bool AtlasPh::get_water_potential_hydrogen(std_msgs::Float32 &msg) {
  msg.data = _water_potential_hydrogen;
  bool res = _send_water_potential_hydrogen;
  _send_water_potential_hydrogen = false;
  return res;
}

void AtlasPh::set_midpoint_calibration(std_msgs::Float32 msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[14];
  sprintf(buf, "Cal,mid,%.2f", msg.data);
  Wire.print(buf);
  Wire.endTransmission();
}

void AtlasPh::set_lowpoint_calibration(std_msgs::Float32 msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[14];
  sprintf(buf, "Cal,low,%.2f", msg.data);
  Wire.print(buf);
  Wire.endTransmission();
}

void AtlasPh::set_highpoint_calibration(std_msgs::Float32 msg) {
  Wire.beginTransmission(_i2c_address);
  char buf[15];
  sprintf(buf, "Cal,high,%.2f", msg.data);
  Wire.print(buf);
  Wire.endTransmission();
}

void AtlasPh::send_query() {
  Wire.beginTransmission(_i2c_address);
  Wire.print("R");
  Wire.endTransmission();
  _waiting_for_response = true;
  _time_of_last_query = millis();
}

void AtlasPh::read_response() {
  Wire.requestFrom(_i2c_address, 20, 1);
  byte response = Wire.read(); // increment buffer by a byte
  String string = Wire.readStringUntil(0);
  status_level = OK;
  status_msg = "";

  // Check for failure
  if (response == 255) {
    status_msg = "No data";
    status_level = ERROR;
  }
  else if (response == 254) {
    status_msg = "Tried to read data before request was processed";
    status_level = ERROR;
  }
  else if (response == 2) {
    status_msg = "Request failed";
    status_level = ERROR;
  }
  else if (response == 1) { // good reading
    _water_potential_hydrogen = string.toFloat();
    _send_water_potential_hydrogen = true;
  }
  else {
    status_msg = "Unknown error";
    status_level = ERROR;
  }
}

/**
 *  \file openag_atlas_ph.cpp
 *  \brief Potential hydrogen sensor.
 */
#include "openag_atlas_ph.h"

AtlasPh::AtlasPh(String id, String* parameters) : Peripheral(id, parameters){
  this->id = id;
  _potential_hydrogen_channel = parameters[0].toInt();
  _potential_hydrogen_key = "potential_hydrogen";
}

AtlasPh::~AtlasPh() {}

void AtlasPh::begin() {
  Wire.begin(); // enable i2c port
  _time_of_last_reading = 0;
}

String AtlasPh::get(String key) {
  if (key == String(_potential_hydrogen_key)) {
    return getPotentialHydrogen();
  }
}

String AtlasPh::set(String key, String value) {
  if (key == String(_potential_hydrogen_key)) {
    return getErrorMessage(_potential_hydrogen_key);
  }
}

String AtlasPh::getPotentialHydrogen(){
  if (millis() - _time_of_last_reading > _min_update_interval){ // can only read sensor so often
    getData();
    _time_of_last_reading = millis();
  }
  return _potential_hydrogen_message;
}

void AtlasPh::getData() {
  boolean is_good_reading = true;

  // Read sensor
  Wire.beginTransmission(_potential_hydrogen_channel); // read message response state
  Wire.print("R");
  Wire.endTransmission();
  delay(1000);
  Wire.requestFrom(_potential_hydrogen_channel, 20, 1);
  byte response = Wire.read(); // increment buffer by a byte
  String string = Wire.readStringUntil(0);

  // Check for failure
  if (response == 255) {
    is_good_reading = false; // no data
  }
  else if (response == 254) {
    is_good_reading = false; // request pending
  }
  else if (response == 2) {
    is_good_reading = false; // request failed
  }
  else if (response == 1) { // good reading
    potential_hydrogen = string.toFloat();
  }
  else {
    is_good_reading = false; // unknown error
  }

  // Update messages
  if (is_good_reading) {
    _potential_hydrogen_message = getMessage(_potential_hydrogen_key, String(potential_hydrogen, 1));
  }
  else { // read failure
    _potential_hydrogen_message = getErrorMessage(_potential_hydrogen_key);
  }
}

String AtlasPh::getMessage(String key, String value) {
  return String(id + "," + key + "," + value);
}

String AtlasPh::getErrorMessage(String key) {
  return String(id + "," + key + ",error");
}

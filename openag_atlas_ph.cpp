/** 
 *  \file openag_atlas_ph.cpp
 *  \brief Potential hydrogen sensor.
 */
#include "openag_atlas_ph.h"

AtlasPh::AtlasPh(String potential_hydrogen_id, int potential_hydrogen_channel) {
  _potential_hydrogen_id = potential_hydrogen_id;
  _potential_hydrogen_channel = potential_hydrogen_channel;
}


void AtlasPh::begin(void) {
  // Enable I2C Port
  Wire.begin();

  // Check For Failure
  Wire.beginTransmission(_potential_hydrogen_channel); // enable message response
  Wire.print("RESPONSE,1");
  Wire.endTransmission(); 
  delay(1000);
  
  Wire.beginTransmission(_potential_hydrogen_channel); // read message response state
  Wire.print("RESPONSE,?");
  Wire.endTransmission(); 
  delay(1000);
  Wire.requestFrom(_potential_hydrogen_channel, 20, 1); 
  String string = Wire.readStringUntil(0);
  
  String ok_string = char(1) + String("?RESPONSE,1"); // verify message response enabled
  if (!string.equals(ok_string)) {
    _sensor_failure = true;
    return;
  }

  // Set Operation Mode
  Wire.beginTransmission(_potential_hydrogen_channel); // disable continuous reading mode
  Wire.print("C,0");
  Wire.endTransmission(); 
  delay(1000);
  
  // Warm Up Sensor
  getPotentialHydrogen();
}


String AtlasPh::get(String id) {
  // Check For Failure
    if (id == _potential_hydrogen_id && _sensor_failure) {
    return String(id + ":" + "error");
  }

  // Respond to Get
  if (id == _potential_hydrogen_id) {
    getPotentialHydrogen();
    return String(id + ":" + String(potential_hydrogen, 1));
  }
}


String AtlasPh::set(String id, String value) {
  // Check For Failure
  if (id == _potential_hydrogen_id && _sensor_failure) {
    return String(id + ":" + "error");
  }

  // Respond to Set
  if (id == _potential_hydrogen_id) {
    return String(id + ":" + "error");
  }
}


void AtlasPh::getPotentialHydrogen(void) {
  Wire.beginTransmission(_potential_hydrogen_channel); // read message response state
  Wire.print("R");
  Wire.endTransmission(); 
  delay(1000);
  Wire.requestFrom(_potential_hydrogen_channel, 20, 1); 
  byte temp = Wire.read(); // increment buffer by a byte
  String string = Wire.readStringUntil(0);
  potential_hydrogen = string.toFloat();
}


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
  Wire.begin();
}


String AtlasPh::get(String id) {
  String message = "";
  if (id == _potential_hydrogen_id) {
    getPotentialHydrogen();
    message = id + ":" + String(potential_hydrogen, 2);
  }
  return message;
}


String AtlasPh::set(String id, String value) {
  return "";
}


void AtlasPh::getPotentialHydrogen(void) {
  char sensor_data[30];
  byte sensor_bytes_received = 0;
  byte code = 0;
  byte in_char = 0;
  
  Wire.beginTransmission(_potential_hydrogen_channel);     // call the circuit by its ID number.
  Wire.write('r');                          // request a reading by sending 'r'
  Wire.endTransmission();                         // end the I2C data transmission.

  delay(1000);  // AS circuits need a 1 second before the reading is ready
  
  sensor_bytes_received = 0;                        // reset data counter
  memset(sensor_data, 0, sizeof(sensor_data));        // clear sensordata array;

  Wire.requestFrom(_potential_hydrogen_channel, 48, 1);    // call the circuit and request 48 bytes (this is more then we need).
  code = Wire.read();

  while (Wire.available()) {          // are there bytes to receive?
    in_char = Wire.read();            // receive a byte.
  
    if (in_char == 0) {               // null character indicates end of command
      Wire.endTransmission();         // end the I2C data transmission.
      break;                          // exit the while loop, we're done here
    }
    else {
      sensor_data[sensor_bytes_received] = in_char;      // append this byte to the sensor data array.
      sensor_bytes_received++;
    }
  }
  potential_hydrogen = atof(sensor_data);
}


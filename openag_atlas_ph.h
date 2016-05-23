/**
 *  \file openag_atlas_ph.h
 *  \brief Potential hydrogen sensor.
 */
#ifndef OPENAG_ATLAS_PH_H
#define OPENAG_ATLAS_PH_H

#include "Arduino.h"
#include <Wire.h>
#include <openag_peripheral.h>

/**
 * \brief Potential hydrogen sensor.
 */
class AtlasPh : public Peripheral {
  public:
    // Public variables
    String id;
    float potential_hydrogen;

    // Public functions
    AtlasPh(String id, String* parameters);
    ~AtlasPh();
    void begin(void);
    String get(String key);
    String set(String key, String value);

  private:
    // Private variables
    int _potential_hydrogen_channel;
    String _potential_hydrogen_message;
    uint32_t _time_of_last_reading;
    const static uint32_t _min_update_interval = 0;
    String _potential_hydrogen_key;

    // Private functions
    void getData();
    String getPotentialHydrogen();
    String getMessage(String key, String value);
    String getErrorMessage(String key);
};

#endif

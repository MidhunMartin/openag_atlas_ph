/**
 *  \file openag_atlas_ph.h
 *  \brief Potential hydrogen sensor.
 */
#ifndef OPENAG_ATLAS_PH_H
#define OPENAG_ATLAS_PH_H

#include "Arduino.h"
#include <Wire.h>
#include <openag_peripheral.h>

#define POTENTIAL_HYDROGEN_KEY "potential_hydrogen"
#define MIN_UPDATE_INTERVAL 0 // milliseconds

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

    // Private functions
    void getData();
    String getPotentialHydrogen();
    String getMessage(String key, String value);
    String getErrorMessage(String key);
};

#endif

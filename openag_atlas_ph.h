/** 
 *  \file openag_atlas_ph.h
 *  \brief Potential hydrogen sensor.
 */
#ifndef OPENAG_ATLAS_PH_H
#define OPENAG_ATLAS_PH_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

/**
 * \brief Potential hydrogen sensor.
 */
class AtlasPh {
  public:
    // Public Functions
    AtlasPh(String potential_hydrogen_id, int potential_hydrogen_channel);
    void begin(void);
    String get(String id);
    String set(String id, String value);

    // Public Variables
    float potential_hydrogen;
    
  private:
    // Private Functions
    void getPotentialHydrogen(void);
    
    // Private Variables
    boolean _sensor_failure = false;
    String _potential_hydrogen_id;
    int _potential_hydrogen_channel;
};

#endif




#include <openag_atlas_ph.h>
AtlasPh atlas_ph("potential_hydrogen", 99);

void setup() {
  Serial.begin(9600);
  atlas_ph.begin();
}

void loop() {
  Serial.println(atlas_ph.get("potential_hydrogen"));
  delay(1000);
}


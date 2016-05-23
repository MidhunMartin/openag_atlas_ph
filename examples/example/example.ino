#include <openag_atlas_ph.h>
String parameters[] = {"99"};
AtlasPh atlas_ph_1("atlas_ph_1", parameters);

void setup() {
  Serial.begin(9600);
  atlas_ph_1.begin();
}

void loop() {
  Serial.println(atlas_ph_1.get("potential_hydrogen"));
  delay(1000);
}

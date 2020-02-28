#ifndef GPS_SUPPORT_CODE
#define GPD_SUPPORT_CODE

void gps(){

  pixel.setPixelColor(0, 255, 0, 0); pixel.show(); // pixel red

  Serial2.begin(9600);
  delay(2000);

  pixel.setPixelColor(0, 255, 255, 0); pixel.show(); // pixel yeller
  Serial2.write(set_gps_fast_serial_cmd);
  delay(500);
  Serial1.end();

  delay(1000);
  Serial2.begin(115200);
  delay(500);
  Serial2.write(set_gps_fast_update_cmd);

  pixel.setPixelColor(0, 0, 255, 0); pixel.show(); // pixel green

  delay(500);
  Serial2.end();
  // END - GPS initialization-------------
}

#endif

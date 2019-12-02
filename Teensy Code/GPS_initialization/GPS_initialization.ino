//This code must be implemented into the teensy that is connected to the GPS module in order to change its communication
//setting so they match the datalogger

void setup() {

  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
  //Serial.begin(9600);
  Serial1.begin(9600);

  //hex code to change the GPS's serial baud rate to 115200
  char a[] = {0x24,0x50,0x4D,0x54,0x4B,0x32,0x35,0x31,0x2C,0x31,0x31,0x35,0x32,0x30,0x30,0x2A,0x31,0x46,0x0D,0x0A};
  //hex code to change the GPS's update rate to 10hz
  char b[] = {0x24,0x50,0x4D,0x54,0x4B,0x32,0x32,0x30,0x2C,0x31,0x30,0x30,0x2A,0x32,0x46,0x0D,0x0A};

  delay(2000);
  Serial1.write(a);
  delay(500);
  Serial1.end();

  delay(1000);
  Serial1.begin(115200);
  delay(500);
  Serial1.write(b);
  delay(500);
  Serial1.end();
  digitalWrite(13, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}

#include <Printers.h>
#include <XBee.h>


//SD Card Library
#include <SD.h>
#include <SD_t3.h>

//Keyboard Library
#include <Keyboard.h>

//SPI Lib
#include <SPI.h>

//ST77XX Lib
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>

//GFX Lib
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include "MSU.cpp"

//XBee Serial Setup
XBee xbee = XBee();
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200,0x4154D778);
XBeeAddress64 broadcast = XBeeAddress64(0x00000000,0x0000FFFF);
ZBTxStatusResponse check = ZBTxStatusResponse();

//TFT Display Board Pins
#define TFT_SCK 13
#define TFT_MOSI 11
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 7

//TFT SD Card Pins
#define TFT_MISO 12
#define TFT_SD_CS 15

//PWM Pin
#define TFT_LITE 6

//Set Up Screen
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// Color definitions
#define BLK    0x0000
#define BLUE   0x001F
#define RED    0xF800
#define GRN    0x07E0
#define CYAN   0x07FF
#define MAG    0xF81F
#define YLW    0xFFE0
#define WHT    0xFFFF

//Button Pins
#define photoGateSignalPin 23

unsigned long lastTime;
float lapTime = 0.0;
float laps[4] = {0.0, 0.0, 0.0, 0.0};
unsigned long printTime;
unsigned long serialTime;
int lapNum = 1;
bool lapNotReset = true;

File Data;
int fileNumber = 0;
String nameString = "";

extern const uint8_t MSU_bits[];
extern const uint8_t FSAE_bits[];


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  xbee.setSerial(Serial1);

  pinMode(TFT_SD_CS,OUTPUT);
  pinMode(TFT_CS,OUTPUT);
  SD.begin(TFT_SD_CS);
  bool fileEval = true;

  for(int i=0; fileEval == true; i++){

    //Create New File Name
    nameString = "Data"+String(i)+".csv";

    //Convert File Name to Character Array for SD Functions
    char fileName[nameString.length()+1];
    nameString.toCharArray(fileName,nameString.length()+1);

    Serial.println(fileName);

    //Test For Files And Set Up Headers
    if (!SD.exists(fileName)){
      Data = SD.open(fileName,FILE_WRITE);        // Create File
      Data.println("Lap,Time(Sec)");              // Make Header. If file is alread created header does not need to be rewritten
      Data.close();                               // Close File
      //fileNumber = i;                           // Dont need with String defined globaly
      fileEval = false;
    }
    else{
      fileEval = true;
    }
  }
  // Opens Up file for writing
  openFile(nameString);

  // First time stoping SD Card
  digitalWrite(TFT_SD_CS, HIGH);

  //Initialize the LCD Screen
  tft.initR(INITR_BLACKTAB);

  //IN/OUT for pins
  pinMode(photoGateSignalPin,INPUT);
  pinMode(TFT_LITE,OUTPUT);

  //Turn on screen
  digitalWrite(TFT_LITE, HIGH);

  //Set time variables
  lastTime = millis();
  printTime = micros();
  serialTime = millis();  //Delay to not burn out Serial Port

  //Text and Screen Layout Properties
  tft.setTextColor(GRN, BLK);
  tft.fillScreen(BLK);
  tft.setRotation(3);
  tft.drawLine(0,30,159,30,GRN);
  tft.drawXBitmap(130,1,FSAE_bits,28,28,GRN); //FSAE Logo


}

void loop() {
  if((digitalRead(photoGateSignalPin) == LOW) && (lapNotReset == true)){
    lapNotReset = false;
  }
  if((digitalRead(photoGateSignalPin) == HIGH) && (lapNotReset != true)){
    Serial.println(lapTime);
    lastTime = millis();
    lapNotReset = true;

    //String for sending to XBee

    //Sending Lap Info as string
    //String lapInfo ="Lap " + String(lapNum) + ": "+ String(lapTime) + "\n";
    //Serial.print(lapInfo);

    //Sending Constant for Lap Trigger
    sendMsg("SR20",addr64); //cannot use broadcast for error checking

    //Keyboard.begin();

    /* Unneeded code to reset SD Card
    SD.begin(TFT_SD_CS);
    //Create File Name
    String nameString = "Data"+String(fileNumber)+".csv";

    //Convert File Name to Character Array for SD Functions
    char fileName[nameString.length()+1];
    nameString.toCharArray(fileName,nameString.length()+1);
    Data= SD.open(fileName,FILE_WRITE);
    Data.print("1");
    Data.close();
    */

    //startScreen();

    // SPI Select Screen
    digitalWrite(TFT_SD_CS, HIGH);
    digitalWrite(TFT_CS, LOW);
    //Clear Screen on new lap
    tft.fillRect(0,37,159,127,BLK);
    tft.fillRect(0,0,129,29,BLK);
    tft.setTextSize(2);

    //Show only last 4 Laps
    for(int i = 3; i > -1; i--){
      if(i != 0){
        laps[i]=laps[i-1];
      }
      else{
        laps[0] = lapTime;
      }
      if(laps[i] != 0.0){
        tft.setCursor(2,37+(24*i));
        tft.print("Lap " + String(lapNum - i) +":");
        tft.print(laps[i],2);
      }
    }
    int pushTime = millis();


    /*
    //Use Keyboard to print to file
    Keyboard.print(laps[0],3);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    Keyboard.end();
    Data.print( "1 \n"); //String(lapNum) + "," + String(laps[0],3)
    Data.flush();
    */


    Serial.println("Done!");

    lapNum++;



    //Delay button Press
    while((millis() - pushTime) <500){
    }
  }

  lapTime = (float)(millis() - lastTime) / 1000.0;

  if((micros() - printTime) > 10000){
    //Set Text Settings for Current Time
    tft.setTextSize(3);
    tft.setCursor(2,2);
    tft.print(lapTime,3);

    //Clocking
    printTime = micros();
  }
  if ((millis()- serialTime) > 100){
    //Put data to print to serial every .1 seconds
  }

}

void startScreen(){
  //Initialize the LCD Screen
  tft.initR(INITR_BLACKTAB);
  //Text and Screen Layout Properties
  tft.setTextColor(GRN, BLK);
  tft.fillScreen(BLK);
  tft.setRotation(3);
  tft.drawLine(0,30,159,30,GRN);
  tft.drawXBitmap(130,1,FSAE_bits,28,28,GRN); //FSAE Logo
}

void sendMsg(String str, XBeeAddress64 adrs){
  //Arrays for message to be sent
  char msgChar[str.length()+1] = {0};
  str.toCharArray(msgChar,str.length()+1);
  uint8_t msg[sizeof(msgChar)]= {0};

  for(int i=0;i<(sizeof(msg)-1);i++){
    msg[i]=msgChar[i];
  }
  //Create Packet to Send Out
  ZBExplicitTxRequest sendOut = ZBExplicitTxRequest(adrs,msg,sizeof(msg));
  xbee.send(sendOut);
  xbee.readPacket();
  xbee.readPacketUntilAvailable();
  Serial.println(xbee.getResponse().getApiId(),HEX);
  if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE){
    xbee.getResponse().getZBTxStatusResponse(check);
    Serial.print("Status: ");
    if(check.getDeliveryStatus() == SUCCESS){
      Serial.println(check.getDeliveryStatus(),HEX);
    }
    else {
      for(int i=0; (i<10 && check.getDeliveryStatus() != SUCCESS);i++){
        xbee.send(sendOut);
        xbee.readPacket();
        xbee.readPacketUntilAvailable();
        xbee.getResponse().getZBTxStatusResponse(check);
        Serial.print("Check ");
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(check.getDeliveryStatus(),HEX);
      }
    }
  }
}

void openFile(String fileTitle){
  // SPI Select SD Card
  digitalWrite(TFT_SD_CS, LOW);
  digitalWrite(TFT_CS, HIGH);

  //Convert File Name to Character Array for SD Functions
  char fileName[fileTitle.length()+1];
  nameString.toCharArray(fileName,fileTitle.length()+1);
  Data= SD.open(fileName,FILE_WRITE);

  // SPI Select Screen
  digitalWrite(TFT_SD_CS, HIGH);
  digitalWrite(TFT_CS, LOW);
  }

  void printLap(int onLap, float timeOfLap) {
    // SPI Select SD Card
    digitalWrite(TFT_SD_CS, LOW);
    digitalWrite(TFT_CS, HIGH);

    String fileOutput = String(onLap) + "," + String(timeOfLap) + "\n";
    Data.print(fileOutput);
    Data.flush();

    // SPI Select Screen
    digitalWrite(TFT_SD_CS, HIGH);
    digitalWrite(TFT_CS, LOW);
  }

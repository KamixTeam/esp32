#include <TinyGPS++.h>
#define RXD2 34
#define TXD2 12
#include <HardwareSerial.h>
#define GPS_SERIAL_NUM 1
 /////////////////////////////saat ve konum bilgisi doğru çalışmaktadır
// The TinyGPSPlus object
TinyGPSPlus gps;
HardwareSerial GPSSerial(GPS_SERIAL_NUM);

struct Signal {
  char sifre[15] = "Fixaj.com";
  char konum[15];
  bool btn1;
  byte btn2[10];
  byte btn3[4];
} data;
 
void setup() {
  Serial.begin(115200);
  GPSSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
 
 
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. "));
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}
 
void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (GPSSerial.available() > 0)
    if (gps.encode(GPSSerial.read()))
      displayInfo();
}
 
void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    *(float*)(data.btn2) = gps.location.lng();
  } else {
    Serial.print(F("INVALID"));
  }
 
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }
 
  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }
 
  Serial.println();
}
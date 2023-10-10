#include <SPI.h>
#include <LoRa.h>

struct Data {
  double x;
  double y;
  char z[10];
};

Data data={70.500444 , 44.500355, "verigeldi"};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(18, 14, 26); // for Lora 32u4
  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("\nSending packet: ");
  Serial.print("x = ");Serial.print(data.x);
  Serial.print(" y = ");Serial.print(data.y);
  Serial.print(" z = ");Serial.print(data.z);  

  // send packet
  LoRa.beginPacket();
  for (unsigned int i = 0; i < sizeof(Data);i++) {
    Serial.print(' ');
    LoRa.write(((byte *) &data)[i]);
    Serial.print(((byte *) &data)[i]);
  }
  LoRa.endPacket();
  

  delay(2000);
}
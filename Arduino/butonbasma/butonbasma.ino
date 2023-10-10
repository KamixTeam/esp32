#include <Arduino.h>
#include "SSD1306Wire.h"      
#define I2C_SDA 21                           // OLED PIN
#define I2C_SCL 22                           // OLED PIN
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL); // Ekran Tanımlama      // OLED ekran için kütüphane


struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {38, 0, false};
void IRAM_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}


void setup() {
    Serial.begin(115200);
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterruptArg(button1.PIN, isr, &button1, FALLING);
}


void loop() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Muhimmat"); 
    display.drawString(0, 18, "Ilk Yardim");
    

    

    if (button1.pressed) {
        //Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;
    }
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 10000) {
      lastMillis = millis();
      detachInterrupt(button1.PIN);
    }
    if(button1.numberKeyPresses % 2 == 1){
      if(button1.pressed){Serial.printf("1 Kez Basıldı %u times\n", button1.numberKeyPresses);}
      display.drawString(0, 0, "Muhimmat <-");
      
    }
    else if(button1.numberKeyPresses % 2 == 0)
    { 
      if(button1.pressed){Serial.printf("2 Kez Basıldı %u times\n", button1.numberKeyPresses);}
      display.drawString(0, 18, "Ilk Yardim <-");
    }
    display.display();
}
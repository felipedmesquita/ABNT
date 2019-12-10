#include <Keyboard.h>
#include <EEPROM.h>
#include "HX711.h"
#include <movingAvg.h>
#include <Esplora.h>

movingAvg  media(50);


const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 16;
int threshold = 300;

long int diff;
long reading1;
HX711 scale;

void setup() {
  Esplora.writeRGB(255, 255, 255);
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  media.begin();
  Keyboard.begin();
  Serial.println(EEPROM.read(0));
  if(EEPROM.read(0) > 10) {
    threshold = EEPROM.read(0)*2;
  }
  
  while (diff > threshold) {
    long reading1 = scale.read() - 800000;
    diff = abs(reading1 - media.reading(reading1));
  }
  Esplora.writeRGB(0, 255, 0);
  delay(500);
}

void loop() {
//Serial.println('.');
  if (scale.is_ready()) {//
    Esplora.writeRGB(0, 0, 10);
    reading1 = scale.read() - 800000;
    diff = abs(reading1 - media.reading(reading1));
    Serial.print(diff);
    Serial.print(", ");
    Serial.println(threshold);

    if(Serial.available()>0) {
      char a = Serial.read();
      switch(a) {
        case 'u':
        threshold+=4;
        EEPROM.write(0, threshold/2);
        break;

        case 'd':
        threshold-=4;
        EEPROM.write(0, threshold/2);
        break;
      }
    }

    
    if (diff > threshold) {
      Esplora.writeRGB(10, 0, 0);
      Keyboard.press('s');
      delay(20);
      Keyboard.release('s');
      delay(20000);//aa
      Keyboard.press('a');
      delay(20);
      Keyboard.release('a');
      Keyboard.releaseAll();
      while (diff > threshold) {
        reading1 = scale.read() - 800000;
        diff = abs(reading1 - media.reading(reading1));
      }

    }
  }

  delay(10);

}

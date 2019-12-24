#include <FastLED.h>
#include <BluetoothSerial.h>

#define NUM_LEDS 132
#define DATA_PIN 33
#define COLOR_ORDER GRB                                   
#define LED_TYPE WS2811
#define FRAMES_PER_SECOND 120

uint8_t brightness = 100; 

unsigned long mark;
byte colorCounter = 17;
int modesQuantity = 25;
byte currentColor = 0;

byte redColor = 0;
byte greenColor = 0;
byte blueColor = 0;

String BTData = "";
boolean recieved;


CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];                      

BluetoothSerial BLUETOOTH;

void setup() {
  Serial.begin(115200);
  BLUETOOTH.begin("Qudi LED stick");
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  
  LEDS.setBrightness(brightness);
  fill_solid( leds, NUM_LEDS, CHSV(150, 250, 250));
}

void loop() {
  checkButton();
  checkBluetooth();
  checkSettings();
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
} 

bool checkButton() {
  if (digitalRead(4) == HIGH && (millis() - mark) > 350) {
    mark = millis();
    currentColor++;
    if(currentColor == modesQuantity) {
      currentColor = 0;
    }
    fill_solid( leds, NUM_LEDS, CHSV(currentColor * 10, 250, 250));
    return true;
  } else { 
    return false; 
  }
}

void checkBluetooth() {
  if(BLUETOOTH.available()) {
    BTData = "";
  }
  while(BLUETOOTH.available()){
    char newData = BLUETOOTH.read();
    BTData = BTData + newData; 
    Serial.println(newData);
    recieved = true;
  }
}

void checkSettings() {
    if (recieved) {
      int colorIndex = 0;
      recieved = false;
      
      while(BTData.indexOf(",")!=-1){
        if(colorIndex == 0) {
          redColor = BTData.substring(0,BTData.indexOf(",")).toInt();
        } else if (colorIndex == 1) {
          greenColor = BTData.substring(0,BTData.indexOf(",")).toInt();
        } else if (colorIndex == 2) {
          blueColor = BTData.substring(0,BTData.indexOf(",")).toInt();
        }
        colorIndex++;
        BTData = BTData.substring(BTData.indexOf(",")+1);
      }
      brightness = round(BTData.toInt());

      LEDS.setBrightness(brightness);
      fill_solid( leds, NUM_LEDS, CRGB(greenColor, redColor, blueColor));
  }
}

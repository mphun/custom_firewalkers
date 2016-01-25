#include <Adafruit_NeoPixel.h>

const int analogInPin = A9;  // Analog input pin that the potentiometer is attached to
int lednum = 40;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(lednum, 6, NEO_GRB + NEO_KHZ800);
int sensorValue = 0;        // value read from the pot
int choice = 0;
int sensitivity = 100;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(9, INPUT_PULLUP); 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);              
  // print the results to the serial monitor:
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);      

  if (sensorValue < sensitivity){
    Serial.println("leds triggered");
    int red = random(255);
    int green = random(255);
    int blue = random(255);
    switch (choice % 6){
      case 0:
        //colorWipe(strip.Color(255, 0, 0), 25);
        colorWipe(strip.Color(red, green, blue), 25);
        colorWipe(strip.Color(0, 0, 0), 25);
        break;
      case 1:
        colorOddEven(strip.Color(red, green, blue), 100, 15);
        clearLed();
        break;
      case 2:
        colorDim(25);
        clearLed();
        break;
      case 3:
        colorWipeBothWay(strip.Color(red, green, blue), 25);
        break;
      case 4:
        rainbow(5);
        clearLed();
        break;
      case 5:
        rainbowCycle(1);
        clearLed();
        break;
    }
    choice++;
    while (sensorValue < sensitivity) {
      sensorValue = analogRead(analogInPin);
      Serial.print("sensor = " );                       
      Serial.println(sensorValue);      
      delay(50);  
    }
  }                 
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorWipeBothWay(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
  for(int i=strip.numPixels(); i >= 0; i--) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
      delay(wait);
  }
}

void colorOddEven(uint32_t c, uint8_t wait, uint8_t cycle){
  uint32_t blank = strip.Color(0, 0, 0);
  for(uint16_t i=0; i<cycle; i++) {
    for(uint16_t j=0; j<strip.numPixels(); j++) {
      if ((j % 2) == (i % 2)){
        strip.setPixelColor(j, c);
      }
      else{
       strip.setPixelColor(j, blank); 
      }
    }
    strip.show();
    delay(wait);
  }
}

void colorDim(uint8_t wait) {
  float red = random(32);
  float green = random(32);
  float blue = random(32);
  float orig_red = red;
  float orig_green = green;
  float orig_blue = blue;
  while (true){
    red = red + (red * 0.1);
    green = green + (green * 0.1);
    blue = blue + (blue * 0.1);
    if ( red > 255 || green > 255 || blue > 255 ){
      break;
    } 
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
    delay(wait);    
  }
  while (true){
    red = red - (red * 0.1);
    green = green - (green * 0.1);
    blue = blue - (blue * 0.1);
    if ( red < orig_red || green < orig_green || blue < orig_blue ){
      break;
    } 
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
    delay(wait);    
  }  
}

void clearLed(){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));      
  }
  strip.show();
  delay(25); 
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
      for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


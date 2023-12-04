#include <Wire.h>

const int analogPin = A0; // Change this to your analog pin
const int i2cAddress = 0x08; // I2C address of the MKR1000
const unsigned long interval = 100; // Interval to send data (in milliseconds)

unsigned long previousMillis = 0;

void setup() {
  Wire.begin(i2cAddress); // Join I2C bus with specified address
  Wire.onRequest(requestEvent); // Register event for I2C request
  pinMode(analogPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  //int analogValue = analogRead(analogPin);
  //Serial.println(analogValue);
  delay(200) ;
}


void requestEvent() {
  int analogValue = analogRead(analogPin);
  Wire.write(lowByte(analogValue));  // Send low byte
  Wire.write(highByte(analogValue));  // Send low byte
  Serial.println(analogValue);
  float dist = 29988 * pow(analogValue, -1.173);
  Serial.print("  ");
  Serial.println(dist);
  
}

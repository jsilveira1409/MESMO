// Define LED pins
const int ledPins[] = {2, 3, 4, 5};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
const int ldrPin = A6;
uint8_t buffer[10];  // Buffer to store incoming data
int bytesRead = 0;    // Number of bytes read
uint8_t cmd = 0;
String msg;
int ldrValue = 0;


// enum for servo's actions
enum{
  LED_PATTERN_1 = 0x01,
  LED_PATTERN_2 = 0x02,
  LED_PATTERN_3 = 0x03,
  READ_LDR = 0x04,
};

void setup() {
  // Set the LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200); // opens serial port, sets data rate to 9600 bps
  Serial.println("Starting System");
  pattern1();
  pattern2();
  pattern3();
}
           

void loop() {
  ldrValue = analogRead(ldrPin);
  

  if (Serial.available() > 0) {
      int servo = 0; // for incoming serial data
      int data = 0;
      bytesRead = Serial.readBytes(buffer, 10);  // Read 8 bytes into buffer
      cmd = buffer[9];

      data = buffer[8];

      switch(cmd){
        case LED_PATTERN_1:
          msg = "Running Patter 1";
          pattern1();
          break;

        case LED_PATTERN_2:
          msg = "Running Pattern 2";
          pattern2();
          break;

        case LED_PATTERN_3:
          msg = "Running Pattern 3";
          pattern3();
          break;

        case READ_LDR:
            msg = "LDR Value at " + String(ldrValue);
            break;

        default:
          Serial.print("Message Unknown: ");
          for (int i = 0; i < 10;i ++){
            Serial.print(buffer[i], HEX);  
          }
          Serial.println();
          break;
      };

      int msg_size = msg.length();
      msg = String(0xdeadbeef, HEX) + String(msg_size, HEX) + msg;            
      Serial.println(msg);
      msg = "";
  }  
}



void pattern1(){
 // Pattern 1: Turn on each LED one by one
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(200);
    digitalWrite(ledPins[i], LOW);
  }
}

void pattern2(){
  // Pattern 2: Turn on all LEDs at once, then turn them off one by one
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
    delay(200);
  }
}

void pattern3(){
  // Pattern 3: Turn on LEDs in an alternate pattern
  for (int i = 0; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 1; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(500);
  for (int i = 1; i < numLeds; i += 2) {
    digitalWrite(ledPins[i], LOW);
  }
}
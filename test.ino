// Define pins for the LEDs
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;
const int pirPin = 5;
const int  ldrPin = A0; 


// Enum for the commands
enum Command {
  TOGGLE_LED1 = 0x1,
  TOGGLE_LED2 = 0x2,
  TOGGLE_LED3 = 0x3, 
  SEND_TELEMETRY = 0x4,
  STOP_TELEMETRY = 0x5,
  NONE
};

bool pirState = false;
bool sendTelemetry = false;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Set the LED pins as outputs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ldrPin,  INPUT);   
}

void loop() {
  // Check if data is available to read from the serial port
  String message = "";
  if (Serial.available()) {
    // Read the incoming byte
    byte receivedCommand = Serial.read();
  
    // Check the command and toggle the corresponding LED
    switch (receivedCommand) {
      case TOGGLE_LED1:
        toggleLED(ledPin1);
        if (sendTelemetry){
          message.concat("Toggled LED1");          
        }
        break;
      case TOGGLE_LED2:
        toggleLED(ledPin2);
        if (sendTelemetry){
          message.concat("Toggled LED2");
        }
        break;
      case TOGGLE_LED3:
        toggleLED(ledPin3);
        if (sendTelemetry){
          message.concat("Toggled LED3");
        }
        break;
      case SEND_TELEMETRY:
        sendTelemetry = true;
        break;
      case STOP_TELEMETRY:
        sendTelemetry = false;
        break;
      default:
        
        break;
    }
    receivedCommand = NONE;
  }
  bool pirValue = digitalRead(pirPin);
  int  ldrStatus = analogRead(ldrPin);   //read the status of the LDR value
  message.concat(pirValue);
  message.concat(ldrStatus);
  if (sendTelemetry){
    Serial.println(message);
  }
  
}

// Function to toggle an LED
void toggleLED(int ledPin) {
  // Read the current state of the LED and toggle it
  if (digitalRead(ledPin) == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

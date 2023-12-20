/* Servo motor with Arduino example code. Position and sweep. More info: https://www.makerguides.com/ */

// Include the servo library:
#include <Servo.h>
#include <string.h>
// enum for servo's actions
enum{
  SERVO_SHOULDER_SET = 0x01,
  SERVO_ELBOW_SET = 0x02,
  SERVO_FOREARM_SET = 0x03,
  SERVO_WRIST_SET = 0x04,
  SERVO_SHOULDER_SWEEP = 0x05,
  SERVO_ELBOW_SWEEP = 0x06,
  SERVO_FOREARM_SWEEP = 0x07,
  SERVO_WRIST_SWEEP = 0x08,
  SERVO_SHOULDER_SPEED = 0x09,
  SERVO_ELBOW_SPEED = 0x0A,
  SERVO_FOREARM_SPEED = 0x0B,
  SERVO_WRIST_SPEED = 0x0C
};

// Create a new servo object:
Servo servoShoulder;
Servo servoElbow;
Servo servoForearm;
Servo servoWrist;

// Define the servo pins for the arm
#define servoElbowPin 9
#define servoShoulderPin 10
#define servoForearmPin 11
#define servoWristPin 12

void setup() {
  // Attach the Servo variable to a pin:
  servoShoulder.attach(servoShoulderPin);
  servoElbow.attach(servoElbowPin);  
  servoForearm.attach(servoForearmPin);  
  servoWrist.attach(servoWristPin);  

  Serial1.begin(115200); 
  servoShoulder.write(70);
  servoElbow.write(70);
  servoForearm.write(70);
  servoWrist.write(70);

}

int shoulderAngle = 0;
int elbowAngle = 0;
int forearmAngle = 0;
int wristAngle = 0;

float shoulderSpeed = 5;    // degrees per seconds
float elbowSpeed = 5;       // degrees per seconds
float forearmSpeed = 5;    // degrees per seconds
float wristSpeed = 5;       // degrees per seconds

float shoulderDelay = 100;  // milliseconds
float elbowDelay = 100;  // milliseconds
float forearmDelay = 100;  // milliseconds
float wristDelay = 100;  // milliseconds

String msg;
uint8_t buffer[10];  // Buffer to store incoming data
int bytesRead = 0;    // Number of bytes read


void loop() {
  
  if (Serial1.available() > 0) {
      int servo = 0; // for incoming serial data
      int data = 0;
      bytesRead = Serial1.readBytes(buffer, 10);  // Read 10 bytes into buffer
      // first byte in message chooses which servo to set to the specified angle
      servo = buffer[8];
      // second byte in message sets the angle to set it to
      data = buffer[9];
      switch(servo){
        case SERVO_SHOULDER_SET:
          shoulderAngle = data;
          servoShoulder.write(shoulderAngle);
          break;

        case SERVO_ELBOW_SET:
          elbowAngle = data;
          servoElbow.write(elbowAngle);
          break;
        
        case SERVO_FOREARM_SET:
          forearmAngle = data;
          servoForearm.write(forearmAngle);
          break;

        case SERVO_WRIST_SET:
          wristAngle = data;
          servoWrist.write(wristAngle);
          break;
        
        case SERVO_SHOULDER_SWEEP:
          sweepServo(servoShoulder, shoulderAngle, data, shoulderDelay);
          break;

        case SERVO_ELBOW_SWEEP:
          sweepServo(servoElbow, shoulderAngle, data, elbowDelay);
          break;

        case SERVO_FOREARM_SWEEP:
          sweepServo(servoForearm, shoulderAngle, data, forearmDelay);
          break;

        case SERVO_WRIST_SWEEP:
          sweepServo(servoWrist, shoulderAngle, data, wristDelay);
          break;
        
        case SERVO_SHOULDER_SPEED:
          // data is in degrees per second
          // delay is in milliseconds
          shoulderDelay = 1000/(float)data;
          break;

        case SERVO_ELBOW_SPEED:
          elbowDelay = 1000/(float)data;
          break;

        case SERVO_FOREARM_SPEED:
          // data is in degrees per second
          // delay is in milliseconds
          forearmDelay = 1000/(float)data;
          break;

        case SERVO_WRIST_SPEED:
          wristDelay = 1000/(float)data;
          break;


        default:
          shoulderAngle = 70;
          elbowAngle = 70;
          forearmAngle = 70;
          wristAngle = 70;
          servoShoulder.write(shoulderAngle);
          servoElbow.write(elbowAngle);
          servoForearm.write(forearmAngle);
          servoWrist.write(wristAngle);
          break;
      };
  }  
}

void sweepServo(Servo &servo, int &currentAngle, int targetAngle, float &delayTime) {
  if (targetAngle < currentAngle) {
    for (; currentAngle >= targetAngle; currentAngle--) {
      servo.write(currentAngle)
      ;
      delay(delayTime);
    }
  } else if (targetAngle > currentAngle) {
    for (; currentAngle <= targetAngle; currentAngle++) {
      servo.write(currentAngle);
      delay(delayTime);
    }
  }
}


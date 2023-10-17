#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <WiFi101.h>
 
char ssid[] = "Jojo";       //  your network SSID (name)
char pass[] = "legrandinquisiteur";   // your network password
int ledpin = 6;           //The built in LED pin on the MKR1000
const int led1 = 0;
const int led2 = 1;
const int led3 = 2;
const int led4 = 3;

const int ledPins[] = {led1, led2, led3, led4};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

int status = WL_IDLE_STATUS;
WiFiServer server(80);
 
enum cmds {
  TOGGLE_LED1 = 0x01,
  TOGGLE_LED2 = 0x02,
  TOGGLE_LED3 = 0x03,
  TOGGLE_LED4 = 0x04,
  SEND_STATUS = 0x05
};

void setup() {
  Serial.begin(9600);      // initialize serial communication
  Serial.print("Start Serial ");
  pinMode(ledpin, OUTPUT);      // set the LED pin mode
  pinMode(led1, OUTPUT);      // set the LED pin mode
  pinMode(led2, OUTPUT);      // set the LED pin mode
  pinMode(led3, OUTPUT);      // set the LED pin mode
  pinMode(led4, OUTPUT);      // set the LED pin mode

  // Check for the presence of the shield
  Serial.print("WiFi101 shield: ");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("NOT PRESENT");
    return; // don't continue
  }
  Serial.println("DETECTED");
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    digitalWrite(ledpin, LOW);
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);
    digitalWrite(ledpin, HIGH);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  digitalWrite(ledpin, HIGH);
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  
  if (client) {            // check if a client is connected
    Serial.println("new client");           // print a message out the serial port
    
    // Keep the connection open until the client disconnects or there's no data for a timeout duration
    unsigned long lastDataTime = millis();
    const unsigned long timeoutDuration = 5000;  // 5 seconds timeout

    while (client.connected() && (millis() - lastDataTime < timeoutDuration)) {
      if (client.available()) {
        lastDataTime = millis();  // Update the last data time

        char c = client.read();             // read a byte
        Serial.print(c, HEX);               // print it out in hexadecimal format

        cmds command = static_cast<cmds>(c);  // Cast the received char to the enum type
        switch (command) {
          case TOGGLE_LED1:
              Serial.println("Led1");
              digitalWrite(led1, !digitalRead(led1));  // Toggle LED1
              break;
          case TOGGLE_LED2:
              Serial.println("Led2");
              digitalWrite(led2, !digitalRead(led2));  // Toggle LED2
              break;
          case TOGGLE_LED3:
              Serial.println("Led3"); 
              digitalWrite(led3, !digitalRead(led3));  // Toggle LED3
              break;
          case TOGGLE_LED4:
              Serial.println("Led4");
              digitalWrite(led4, !digitalRead(led4));  // Toggle LED4
              break;
          case SEND_STATUS:
              // Handle SEND_STATUS command
              client.write("Doing Great!");
              // For example, you can send back the status of the LEDs or any other information
              break;
          default:
              Serial.println("Unknown command received.");
              Serial.println();
              break;
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    }
    chasePattern();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void chasePattern() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);  // Turn on the current LED
    delay(100);                      // Wait for 200ms
    digitalWrite(ledPins[i], LOW);   // Turn off the current LED

    // If it's the last LED in the sequence, delay a bit longer before restarting the pattern
    if (i == numLeds - 1) {
      delay(250);
    }
  }
}

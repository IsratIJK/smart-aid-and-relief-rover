#include <SPI.h>
#include <MFRC522.h>
#include "thingProperties.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <Servo.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_SSID_PASSWORD";
String phoneNumber = "WHATSAPP_SIM_NUMBER_FOR_API";
String apiKey = "API_KEY";

#define SS_PIN D4   // D8 on NodeMCU
#define RST_PIN D3  // D3 on NodeMCU
const int trigPin = D1; 
const int echoPin = D2; 
long duration;
int dist;

Servo mservo;


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create instance of MFRC522

void sendMessage(String message){
 
  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);
 
  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
 
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  // else{
  //   Serial.println("Error sending the message");
  //   Serial.print("HTTP response code: ");
  //   Serial.println(httpResponseCode);
  // }
 
  // Free resources
  http.end();
}

void setup() {
  Serial.begin(9600);    // Initialize serial communication
  SPI.begin();           // Initialize SPI bus
  mfrc522.PCD_Init();    // Initialize MFRC522
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Serial.println("RFID Scanning Module Initialized!");
  // Serial.println("Place your RFID card near the reader...");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  mservo.attach(D8);
  mservo.write(0);
}

void loop() {
  ArduinoCloud.update();

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("A new card has been detected!");

    Serial.print("Card UID: ");
    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uidString += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uidString += String(mfrc522.uid.uidByte[i], HEX);
    }
    rFID = "Tag: "+ uidString; // Update the RFID variable for IoT Cloud
    Serial.println(uidString);

    
    total_people++;

    // Halt PICC
    mfrc522.PICC_HaltA();
  }
  delay(100);
  
   // Clears the trigPin
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
// Calculating the distance
 dist = duration*0.034/2;
// Serial.println(dist);
 storage_condition=dist;
 if(storage_condition > 8)
{
  sendMessage("Storage is low!");
  mservo.write(48); // rotate
  delay(1000);
  mservo.write(0);  // stop
  delay(500);

}

}

/*
  Since RFID is READ_WRITE variable, onRFIDChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onRFIDChange()  {
  // Add your code here to act upon RFID change
}

/*
  Since TotalPeople is READ_WRITE variable, onTotalPeopleChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTotalPeopleChange()  {
  // Add your code here to act upon TotalPeople change
}
/*
  Since StorageCondition is READ_WRITE variable, onStorageConditionChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onStorageConditionChange()  {
  // Add your code here to act upon StorageCondition change
}
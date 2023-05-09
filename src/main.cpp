#include <Arduino.h>
#include "ESP8266WiFi.h"
#include <Firebase_ESP_Client.h>
#include <SoftwareSerial.h>

const char* ssid = "KORMAN";
const char* pass = "kmx1609@$1";

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyCi4-N7y-PpMMh3Ay6Way3wEPGrS0N40hw"
#define DATABASE_URL "https://hermes-26852-default-rtdb.firebaseio.com/" 

#define USER_EMAIL "hermes@teste.com"
#define USER_PASSWORD "38131883"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

SoftwareSerial serial(2,3);

String uid;

void setup() {
  
  Serial.begin(9600);
  serial.begin(9600);
  WiFi.begin(ssid, pass);

  Serial.print("Conectando");
  while ( WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to WiFi.IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);

  while ((auth.token.uid) == "") {
    Serial.print(".");
    delay(1000);
  }

  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);

  Firebase.RTDB.setString(&fbdo, "Teste/Robo", "Test");
}

void loop() {
  if (serial.available() > 0) {
    String dado = serial.readStringUntil('\n');
    if (strcmp(dado.c_str(), "click") == 0){
      Serial.println("Click");
      Firebase.RTDB.setString(&fbdo, "/Test/Robo", "click");
    } else if (strcmp(dado.c_str(), "hold") == 0){
      Serial.println("Hold");
      Firebase.RTDB.setString(&fbdo, "/Test/Robo", "hold");
    } else {
      Serial.println("Erro");
    }
  }
}
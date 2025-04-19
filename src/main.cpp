#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h> // Include the WiFiManager library
#include <EEPROM.h>

WiFiManager wifiManager; // Create an instance of WiFiManager

// masukkan token dan template id blynk kalian
#define BLYNK_TEMPLATE_ID "TMPL6QCrULozi"
#define BLYNK_TEMPLATE_NAME "Projek Coba Coba"
#define BLYNK_AUTH_TOKEN "Gq08VKO8VNHrUMROialSfuXcLuuKHNpe"

// pin relay pada nodemcu
const int relay1Pin = D1; // Relay for Blynk V1
const int relay2Pin = D4; // Relay for Blynk V2
int led1State, led2State;

void saveRelayStateToEEPROM() {
  EEPROM.write(1, led1State);
  EEPROM.write(2, led2State);
  EEPROM.commit();
  Serial.println("Relay states saved to EEPROM.");
}

void loadRelayStateFromEEPROM() {
  led1State = EEPROM.read(1);
  led2State = EEPROM.read(2);
  Serial.println("Relay states loaded from EEPROM.");
}

void setup() {
  EEPROM.begin(5); // Initialize EEPROM with size 5
  loadRelayStateFromEEPROM(); // Load relay states from EEPROM

  // Set relay pins to the last saved state
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, led1State);
  digitalWrite(relay2Pin, led2State);

  // Menyalakan WiFi untuk konfigurasi
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  // wm.resetSettings(); // jika dinyalakan, maka semua settingan wifi yang tersimpan akan dihapus
  bool res;
  res = wm.autoConnect("NodeMcu", "123456789");
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    // if you get here you have connected to the WiFi
    Serial.println("terhubung :)");
  }

  Serial.begin(9600);
  Blynk.config(BLYNK_AUTH_TOKEN); // Blynk V2
}

BLYNK_WRITE(V1) { // Tombol virtual untuk relay 1
  led1State = param.asInt();
  digitalWrite(relay1Pin, led1State);
  saveRelayStateToEEPROM(); // Save state to EEPROM
  Serial.print("Relay 1 is now ");
  Serial.println(led1State ? "ON" : "OFF");
}

BLYNK_WRITE(V2) { // Tombol virtual untuk relay 2
  led2State = param.asInt();
  digitalWrite(relay2Pin, led2State);
  saveRelayStateToEEPROM(); // Save state to EEPROM
  Serial.print("Relay 2 is now ");
  Serial.println(led2State ? "ON" : "OFF");
}

void loop() {
  Blynk.run();
}
/*
 * WiFi Credentials Writer for ESP32-S3
 * Flash this program once to store WiFi credentials in NVS
 * After flashing, immediately upload the second program
 */

#include <Preferences.h>

Preferences prefs;

// ============================================
// CONFIGURE YOUR WIFI CREDENTIALS HERE
// ============================================
const char* WIFI_SSID = "Ramu";
const char* WIFI_PASSWORD = "9807567941";
// ============================================

void setup() {
  Serial.begin(115200);
  delay(2000);  // Wait for serial monitor
  
  Serial.println("\n\n==================================");
  Serial.println("WiFi Credentials Writer for ESP32-S3");
  Serial.println("==================================\n");
  
  // Open Preferences in read-write mode
  Serial.println("Opening Preferences...");
  prefs.begin("wifi_creds", false);
  
  // Save WiFi credentials
  Serial.println("Saving WiFi credentials to NVS...");
  prefs.putString("ssid", WIFI_SSID);
  prefs.putString("password", WIFI_PASSWORD);
  
  // Optional: Store additional info
  prefs.putBool("configured", true);
  prefs.putUInt("save_time", millis());
  
  Serial.println("✓ Credentials saved successfully!");
  Serial.println("\nStored credentials:");
  Serial.print("  SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("  Password: ");
  Serial.println(WIFI_PASSWORD);
  
  // Close preferences
  prefs.end();
  
  Serial.println("\n==================================");
  Serial.println("IMPORTANT: Now upload the WiFi");
  Serial.println("connection program to use these");
  Serial.println("credentials!");
  Serial.println("==================================\n");
  
  // Blink LED to show completion
  // pinMode(LED_BUILTIN, OUTPUT);
  // for(int i = 0; i < 10; i++) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(200);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(200);
  // }
}

void loop() {
  // Nothing to do here
  delay(1000);
}
/*
 * WiFi Connection Program for ESP32-S3
 * Reads WiFi credentials from NVS and connects
 * This is your main application program
 */

#include <Preferences.h>
#include <WiFi.h>

Preferences prefs;

// WiFi credentials (will be loaded from NVS)
String wifi_ssid;
String wifi_password;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n==================================");
  Serial.println("WiFi Connection Program");
  Serial.println("==================================\n");
  
  // Load WiFi credentials from NVS
  if (loadWiFiCredentials()) {
    Serial.println("✓Credentials loaded from NVS");
    Serial.print("  SSID: ");
    Serial.println(wifi_ssid);
    Serial.println("  Password: ********");
    
    // Connect to WiFi
    connectToWiFi();
  } else {
    Serial.println("✗ ERROR: No WiFi credentials found!");
    Serial.println("Please upload the credentials writer program first.");
  }
}

void loop() {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected - IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("WiFi Disconnected! Reconnecting...");
    connectToWiFi();
  }
  
  delay(10000);  // Check every 10 seconds
}

bool loadWiFiCredentials() {
  prefs.begin("wifi_creds", true);  // Read-only mode
  
  // Check if credentials exist
  if (!prefs.isKey("ssid")) {
    prefs.end();
    return false;
  }
  
  // Load credentials
  wifi_ssid = prefs.getString("ssid", "");
  wifi_password = prefs.getString("password", "");
  
  prefs.end();
  
  // Validate
  return (wifi_ssid.length() > 0);
}

void connectToWiFi() {
  Serial.println("\nConnecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(wifi_ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✓ WiFi Connected!");
    Serial.print("  IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("  MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("  Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("✗ WiFi Connection Failed!");
    Serial.println("  Check credentials and network availability");
  }
}
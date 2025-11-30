/*
author Ramu acharya
EVON-ELECTRIC
*/

#include <Arduino.h>
void Task1(void *parameter) {
  for(;;) {
    Serial.println("hello from Core " + String(xPortGetCoreID()));
    vTaskDelay(500 / portTICK_PERIOD_MS);  // 500ms delay
  }
}

void Task2(void *parameter) {
  for(;;) {
    Serial.println("RAMU From Core " + String(xPortGetCoreID()));
    vTaskDelay(500 / portTICK_PERIOD_MS);  // 500ms delay
  }
}

void setup() {
  Serial.begin(115200);
  
  // Create Task 1 on Core 0
  xTaskCreatePinnedToCore(
    Task1,      // Function
    "Task1",    // Name
    10000,      // Stack size
    NULL,       // Parameters
    1,          // Priority
    NULL,       // Handle
    0           // Core 0
  );
  
  // Create Task 2 on Core 0
  xTaskCreatePinnedToCore(
    Task2,      // Function
    "Task2",    // Name
    10000,      // Stack size
    NULL,       // Parameters
    1,          // Priority
    NULL,       // Handle
    0        // Core 0
  );
}

void loop(){
  // Empty loop
}
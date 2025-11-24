#include <Arduino.h>

// Task 1 - runs on Core 0
void Task1(void *parameter) {
  for(;;) {
    Serial.println("Task 1 running on Core " + String(xPortGetCoreID()));
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1 second delay
  }
}

// Task 2 - runs on Core 1
void Task2(void *parameter) {
  for(;;) {
    Serial.println("Task 2 running on Core " + String(xPortGetCoreID()));
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
  
  // Create Task 2 on Core 1
  xTaskCreatePinnedToCore(
    Task2,      // Function
    "Task2",    // Name
    10000,      // Stack size
    NULL,       // Parameters
    1,          // Priority
    NULL,       // Handle
    1           // Core 1
  );
}

void loop() {
  // Empty - tasks handle everything
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
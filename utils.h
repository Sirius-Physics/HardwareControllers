#ifndef UTILS_H
#define UTILS_H

#include <ArduinoJson.h>

void saveInitialTime();
unsigned long getCurrentTime();

template<typename T>
void addPairToJson(JsonObject& obj, const char* key, T value) {
  obj[key] = value;
}

// To process arguments pairs
template<typename K, typename V, typename... Rest>
void processPairs(JsonObject& obj, K key, V value, Rest... rest) {
  addPairToJson(obj, key, value);
  if constexpr (sizeof...(rest) > 0) {
    processPairs(obj, rest...);
  }
}

// Create and serialize multiple arguments JSON
template<typename... Args>
void createAndSerializeJson(const char* objectName, Args... args) {
  JsonDocument doc;
  JsonObject obj = doc[objectName].to<JsonObject>();
  
  processPairs(obj, args...);
  
  serializeJson(doc, Serial);
  Serial.println();
}

#endif
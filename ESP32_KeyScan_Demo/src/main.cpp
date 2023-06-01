#include <Arduino.h>
#include "INPUT.h"
#include "key.h"


void setup() {
  key_init();
  Serial.begin(115200);
  Serial.println("esp32 init");
}

void callback(uint8_t id, uint8_t type)
{
  Serial.printf("key_id = %d  pressed_type = %d\r\n",id,type);
}


void loop() {
  Key_Driver(callback);
}
#include <ArduinoJson.h>
#include <Servo.h>

#define CHANNEL_OFFSET 6
#define CHANNELS 4


JsonDocument command;
Servo channelsServos[CHANNELS];

uint8_t commandChannelMode = 0;
uint8_t commandChannelPulse = 0;
uint8_t commandChannelAngle = 0;
uint8_t commandChannelPin = 0;

void channelsInit(Servo *servosArray, uint8_t count, uint8_t offset) {
  for (uint8_t i = 0; i < count; i++) {
    Servo servo;
    servosArray[i] = servo;
    servosArray[i].attach(i + offset);
  }
}



void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  channelsInit(channelsServos, CHANNELS, CHANNEL_OFFSET);

  JsonDocument conf;
  conf["type"] = "config";
  conf["offset"] = CHANNEL_OFFSET;
  conf["count"] = CHANNELS;
  serializeJson(conf, Serial);
}



void loop() {
  if (Serial.available()) {
    String payload = Serial.readStringUntil("\n");
    DeserializationError error = deserializeJson(command, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    // Extract the values
    commandChannelPin = command["ch"];
    commandChannelMode = command["mode"];
    switch (commandChannelMode) {
      // Angle mode
      case 0:
        if ((commandChannelPin + CHANNEL_OFFSET) >= CHANNEL_OFFSET && (commandChannelPin + CHANNEL_OFFSET) <= (CHANNEL_OFFSET + CHANNELS - 1)) {
          channelsServos[commandChannelPin].write(command["ang"]);
        }
        break;
    }

  }

}

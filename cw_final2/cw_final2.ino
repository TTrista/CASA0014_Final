#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <utility/wifi_drv.h> // library to drive to RGB LED on the MKR1010
#include "arduino_secrets.h"

// Sensors pin
#define TRIG_PIN 1
#define ECHO_PIN 2
#define NUM_PIXELS 12 // Define the number of pixels in the light strip

// WiFi and MQTT settings
const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

const char* mqtt_topic_demo = "student/CASA0014/light/12/pixel/"; // Fixed MQTT theme

WiFiClient wificlient;
PubSubClient client(wificlient);

// Distance tracking variables
long lastTime = 0;
float lastDistance = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT
  client.setServer(mqtt_server, mqtt_port);
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  //long currentTime = millis();
  //float speed = abs(distance - lastDistance) / (currentTime - lastTime) * 1000; // cm/s
  //lastDistance = distance;
 // lastTime = currentTime;

  if (distance > 300) {
    sendMQTTMessage(245, 50, 245); // 淡粉色
  } else if (distance > 40 && distance <= 300) {
    sendMQTTMessage(255, 215, 0); // 柠檬黄
  } else if (distance <= 40) {
    sendMQTTMessage(0, 50, 252);
    sendWaveEffect();
     // 海蓝色
  }

  // if (distance <= 40) {
  //   sendWaveEffect();
  // }

  delay(100); // 100ms update interval
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

void sendMQTTMessage(int r, int g, int b) {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) { // 使用 NUM_PIXELS 替代硬编码的 12
    char mqtt_message[100];
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": %d}", pixel,
           r,g,b, 0);

    if (client.publish(mqtt_topic_demo, mqtt_message)) {
      Serial.println("Message published");
    } else {
      Serial.println("Failed to publish message");
    }
    delay(10);
  }
}

void sendWaveEffect() {
  for (int i = 0; i < 120; i++) {
    clearAllPixels();
    char mqtt_message[100];
    sprintf(mqtt_message, "{\"brightness\": %d}", i);

    if (client.publish("student/CASA0014/light/12/brightness/", mqtt_message)) {
      Serial.println("Wave effect sent");
    } else {
      Serial.println("Failed to publish wave effect");
    }
    delay(50); // Control wave speed
  }

  for (int j = 119; j >= 0; j--) {
    //clearAllPixels();
    char mqtt_message[100];
    sprintf(mqtt_message, "{\"brightness\": %d}", j);

    if (client.publish("student/CASA0014/light/12/brightness/", mqtt_message)) {
      Serial.println("Wave effect sent");
    } else {
      Serial.println("Failed to publish wave effect");
    }
    delay(50); // Control wave speed
  }
}

void clearAllPixels() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    char mqtt_message[100];
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": 0, \"G\": 0, \"B\": 0, \"W\": 0}", pixel);
    if (client.publish(mqtt_topic_demo, mqtt_message)) {
      Serial.println("Message published");
    } else {
      Serial.println("Failed to publish message");
    }
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("MKR1010", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
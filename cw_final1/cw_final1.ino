#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <utility/wifi_drv.h> // library to drive to RGB LED on the MKR1010
#include "arduino_secrets.h"

// Sensor pins
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

const char* mqtt_topic_demo = "student/CASA0014/light/51/pixel/"; // Fixed MQTT topic
const char* mqtt_brightness_topic = "student/CASA0014/light/51/brightness/"; // Brightness control topic

WiFiClient wificlient;
PubSubClient client(wificlient);

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

  // Color control based on distance
  if (distance > 300) {
    sendMQTTMessage(245, 50, 245); // Pink
  } else if (distance > 40 && distance <= 300) {
    sendMQTTMessage(255, 215, 0); // Yellow
  } else if (distance <= 40) {
    sendMQTTMessage(0, 50, 252); // Blue
    sendWaveEffect(); // Trigger wave effect
  }

  delay(100); // 100ms update interval
}

// Measure distance using ultrasonic sensor
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

// Send MQTT message to set light color
void sendMQTTMessage(int r, int g, int b) {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    char mqtt_message[100];
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": %d}", pixel, r, g, b, 0);

    if (client.publish(mqtt_topic_demo, mqtt_message)) {
      Serial.print("Message published for pixel ");
      Serial.println(pixel);
    } else {
      Serial.println("Failed to publish message");
    }
    delay(10); // Small delay to avoid overwhelming MQTT broker
  }
}

// Wave effect for blue zone
void sendWaveEffect() {
  // Brightness increase
  for (int i = 0; i <= 120; i++) {
    setBrightness(i);
    delay(50); // Adjust wave speed
  }

  // Brightness decrease
  for (int i = 120; i >= 0; i--) {
    setBrightness(i);
    delay(50); // Adjust wave speed
  }
}

// Set brightness via MQTT
void setBrightness(int brightness) {
  char mqtt_message[50];
  sprintf(mqtt_message, "{\"brightness\": %d}", brightness);

  if (client.publish(mqtt_brightness_topic, mqtt_message)) {
    Serial.println("Brightness updated");
  } else {
    Serial.println("Failed to update brightness");
  }
}

// Reconnect to MQTT broker
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("MKR1010", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

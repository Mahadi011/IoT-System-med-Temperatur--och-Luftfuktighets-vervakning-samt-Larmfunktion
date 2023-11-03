#include <WiFi.h>
#include <PubSubClient.h>
#define Type DTH22
#define dthpin 21

const char *SSID = "your Network address";
const char *PWD = "Your password";

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

//Tempsensor configuration
DHT HT(dhtPin, Type);
float tempC;
int humidity;

long last_time = 0;

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected.");
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);

}

void setup() {
  Serial.begin(115200);
  delay(2000);
  HT.begin();

  connectToWiFi();
  setupMQTT();
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  long now = millis();
  if (now - last_time > 6000) {
    to get the temp and humidity
    tempC = HT.readTemperature();
    humidity = HT.readHumidity();

    char data[100];
    char hum[100];

    sprintf(data, "%.2f", tempC); // Format temperature with two decimal places
    Serial.print("Temperature: ");
    Serial.println(data);
    mqttClient.publish("/iot22tmos/temperature", data);

    sprintf(hum, "%d", humidity); // Format humidity as an integer
    Serial.print("Humidity: ");
    Serial.println(humidity);
    mqttClient.publish("/iot22tmos/humidity", hum);

    last_time = now;
  }
  delay(10000);
}

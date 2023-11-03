#include <WiFi.h>
#include <PubSubClient.h>
#define alarmPin 12

const char *SSID = "Your Network address";
const char *PWD = "your password";

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Receiving from topic:: ");
  Serial.print(topic);
  Serial.print(" Message: ");
 char receivedPayload[length + 1];  // +1 for null-terminator
 String pReceivePayload ="";
 memset(receivedPayload, 0, sizeof(receivedPayload)); // Initialize the buffer

  for (int i = 0; i < length; i++) {
    receivedPayload[i] = (char)payload[i];
  }
  String RP=(String)receivedPayload;
  if(RP!=pReceivePayload){
    digitalWrite(alarmPin,HIGH);
    delay(5000);
    digitalWrite(alarmPin,LOW);
    pReceivePayload=RP;

  }

  // Print the payload data
  Serial.println(receivedPayload);

    

 
  

}

void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // Set the callback function
  mqttClient.setCallback(callback);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(alarmPin,OUTPUT);

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
      // Subscribe to topics
      
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  
   mqttClient.subscribe("/iot22tmos/alarm");
  
  
  
}

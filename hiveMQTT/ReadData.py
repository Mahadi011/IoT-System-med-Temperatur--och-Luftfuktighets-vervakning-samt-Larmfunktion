import paho.mqtt.client as mqtt
import time
from connect_to_mysql import DataHandler
from Write_in_MQTTServer import MQTTDataPublisher


class MQTTDataHandler:
    def __init__(self, mqtt_server, mqtt_port, temperature_topic, humidity_topic):
        self.data_handler = DataHandler()
        self.data_handler.drop_table()
        self.data_handler.create_table()
        self.data_publish=MQTTDataPublisher()
      #  self.data_write=DataPublisher()


        self.mqtt_server = mqtt_server
        self.mqtt_port = mqtt_port
        self.temperature_topic = temperature_topic
        self.humidity_topic = humidity_topic
        self.temperature = None
        self.humidity = None
        self.pTemp=None
        self.pHum=None

        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message

    def on_connect(self, client, userdata, flags, rc):
        print("Connected to MQTT broker with result code " + str(rc))
        client.subscribe(self.temperature_topic)
        client.subscribe(self.humidity_topic)

    def on_message(self, client, userdata, msg):
        if msg.topic == self.temperature_topic:
            self.temperature = float(msg.payload)
        elif msg.topic == self.humidity_topic:
            self.humidity = int(msg.payload)

    def connect_to_mqtt(self):
        self.client.connect(self.mqtt_server, self.mqtt_port, 60)
        self.client.loop_start()
    def check_condition( self):
        if not (27.00 <= self.temperature <= 24.00) or not (80 <= self.humidity <= 70):
            self.data_publish.publish_data("true")


    def start(self):
        self.connect_to_mqtt()
        while True:
            print(f"temperature={self.temperature} degree C")
            print(f"Humidity={self.humidity}%")

            if self.temperature is not None or self.humidity is not None:
                handler.check_condition()
                if self.temperature is not self.pTemp and self.humidity is not self.pHum:
                    self.data_handler.insert_data(self.temperature, self.humidity)
                    self.pTemp=self.temperature
                    self.pHum=self.humidity

            time.sleep(10)

if __name__ == "__main__":
    mqtt_server = "broker.hivemq.com"
    mqtt_port = 1883
    temperature_topic = "/iot22tmos/temperature"
    humidity_topic = "/iot22tmos/humidity"

    handler = MQTTDataHandler(mqtt_server, mqtt_port, temperature_topic, humidity_topic)
    handler.start()

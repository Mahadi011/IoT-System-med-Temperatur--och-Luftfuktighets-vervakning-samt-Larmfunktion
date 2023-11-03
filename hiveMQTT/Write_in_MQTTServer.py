import paho.mqtt.client as mqtt

class MQTTDataPublisher:
    def __init__(self):
        self.mqtt_server ="broker.hivemq.com"
        self.mqtt_port = 1883
        self.topic = "/iot22tmos/alarm"
        self.client = mqtt.Client()
        self.client.connect(self.mqtt_server, self.mqtt_port, 60)

    def publish_data(self, data):
        self.client.publish(self.topic, data)
        print("publish successfully")





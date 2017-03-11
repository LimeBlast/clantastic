import time
import random
import paho.mqtt.client as mqtt
import settings


def main():
    mqttclient = mqtt.Client()
    mqttclient.username_pw_set(settings.MQTT_USERNAME, settings.MQTT_PASSWORD)
    mqttclient.connect(settings.MQTT_HOST, int(settings.MQTT_PORT))

    while True:
        rgb = ['red', 'green', 'blue']
        topic = 'rgb/{}'.format(random.choice(rgb))
        payload = random.randint(0, 255)

        print('[MQTT] Publishing value of {} to channel {}'.format(payload, topic))
        mqttclient.publish(topic, payload)
        time.sleep(1)


if __name__ == '__main__':
    main()

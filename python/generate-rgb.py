import time
import random
import paho.mqtt.client as mqtt
import settings


def on_publish(client, userdata, mid):
    print("Message " + str(mid) + " published.")


def main():
    mqttclient = mqtt.Client()
    mqttclient.username_pw_set(settings.MQTT_USERNAME, settings.MQTT_PASSWORD)
    mqttclient.connect(settings.MQTT_HOST, int(settings.MQTT_PORT))
    mqttclient.on_publish = on_publish

    kids = ['daniel', 'jessica', 'emily']

    while True:
        topic = 'Clantastic/feeds/{}'.format(random.choice(kids))
        payload = random.randint(0, 255)

        print('[MQTT] Publishing value of {} to channel {}'.format(payload, topic))
        mqttclient.publish(topic, payload)
        time.sleep(3)


if __name__ == '__main__':
    main()

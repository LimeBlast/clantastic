/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'daniel' for subscribing to changes.
Adafruit_MQTT_Subscribe daniel  = Adafruit_MQTT_Subscribe(&mqtt, "clantastic/daniel");
Adafruit_MQTT_Subscribe jessica = Adafruit_MQTT_Subscribe(&mqtt, "clantastic/jessica");
Adafruit_MQTT_Subscribe emily   = Adafruit_MQTT_Subscribe(&mqtt, "clantastic/emily");

/******************************* LEDs ***************************************/

#define PIXELS_PIN 15
#define NUM_LEDS 8
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

int red = 255;
int green = 255;
int blue = 255;
char hex[8] = {0};

/*************************** Sketch Code ************************************/

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  lightStrip(red, green, blue);
  
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for feeds.
  mqtt.subscribe(&daniel);
  mqtt.subscribe(&jessica);
  mqtt.subscribe(&emily);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &daniel) {
      Serial.print(F("Daniel got: "));
      Serial.println((char *)daniel.lastread);
      green = (int)daniel.lastread;
    }
    if (subscription == &jessica) {
      Serial.print(F("Jessica got: "));
      Serial.println((char *)jessica.lastread);
      blue = (int)jessica.lastread;
    }
    if (subscription == &emily) {
      Serial.print(F("Emily got: "));
      Serial.println((char *)emily.lastread);
      red = (int)emily.lastread;
    }
  }

  lightStrip(red, green, blue);

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void lightStrip(int r, int g, int b) {
  //change NeoPixel color here using format strip.Color(R,G,B)
  for(int i=0; i<strip.numPixels(); i++) { //turn off all NeoPixels
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show(); //always remember to call strip.show() to display changes
  Serial.print("Setting LED to R: ");
  Serial.print(r);
  Serial.print(", G: ");
  Serial.print(g);
  Serial.print(", B: ");
  Serial.println(b);
}

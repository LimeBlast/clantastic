// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi client.
#include "config.h"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************ Main Program Starts Here *******************************/
#include <ESP8266WiFi.h>
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include <ArduinoHttpClient.h>
#include <Adafruit_NeoPixel.h>

#define PIXELS_PIN 15
#define NUM_LEDS 8
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

int red = 255;
int green = 255;
int blue = 255;
char hex[8] = {0};

// set up the 'family-box' group
AdafruitIO_Group *group = io.group("family-box");

void setup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  lightStrip(red, green, blue);

  // start the serial connection
  Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  // set up a message handlers for the group feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  group->onMessage("daniel", handleMessage);
  group->onMessage("jessica", handleMessage);
  group->onMessage("emily", handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}

// this function is called whenever a 'command' message
// is received from Adafruit IO. it was attached to
// the command feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.print(data->toInt());
  Serial.print(" from ");
  Serial.println(data->feedName());
  
  if (String(data->feedName()).equalsIgnoreCase("daniel")) {
    green = data->toInt();
  } else if (String(data->feedName()).equalsIgnoreCase("jessica")) {
    blue = data->toInt();
  } else if (String(data->feedName()).equalsIgnoreCase("emily")) {
    red = data->toInt();
  }
    
  lightStrip(red, green, blue);
  
  sprintf(hex,"#%02X%02X%02X",red,green,blue);
  Serial.println(hex); //Print the string.

//  group->set("hex", hex);
//  group->save();
}

void lightStrip(int r, int g, int b) {
  //change NeoPixel color here using format strip.Color(R,G,B)
  for(int i=0; i<strip.numPixels(); i++) { //turn off all NeoPixels
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show(); //always remember to call strip.show() to display changes  
}


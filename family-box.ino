// Instructables Internet of Things Class sample code
// Circuit Triggers Internet Action
// A button press is detected and stored in a feed
// An LED is used as confirmation feedback
//
// Modified by Becky Stern 2017
// based on the Adafruit IO Digital Input Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-input
//
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
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/************************ Main Program Starts Here *******************************/
#include <ESP8266WiFi.h>
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include <ArduinoHttpClient.h>

#define BUTTON_PIN 4
#define LED_PIN 13

#include <Adafruit_NeoPixel.h>

#define PIXELS_PIN 15
#define NUM_LEDS 8
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

// button state
int current = 0;
int last = 0;

int red = 0;
int green = 0;
int blue = 0;

// set up the 'command' feed
AdafruitIO_Feed *command = io.feed("command");
AdafruitIO_Group *group = io.group("family-box");

void setup() {

  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // start the serial connection
  Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  // set up a message handler for the 'command' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  command->onMessage(handleMessage);
  group->onMessage("daniel", handleColour);
  group->onMessage("jessica", handleColour);
  group->onMessage("emily", handleColour);

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

  // grab the current state of the button.
  // we have to flip the logic because we are
  // using INPUT_PULLUP.
  if(digitalRead(BUTTON_PIN) == LOW)
    current = 1;
  else
    current = 0;

  // return if the value hasn't changed
  if(current == last)
    return;

  // save the current state to the 'command' feed on adafruit io
  Serial.print("sending button -> ");
  Serial.println(current);
  command->save(current);

  // store last button state
  last = current;
}

// this function is called whenever a 'command' message
// is received from Adafruit IO. it was attached to
// the command feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  int command = data->toInt();

  if (command == 1){ //light up the LED
    Serial.print("received <- ");
    Serial.println(command);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.print("received <- ");
    Serial.println(command);
  }
}

void handleColour(AdafruitIO_Data *data) {
  if (String(data->feedName()).equalsIgnoreCase("daniel")) {
    green = data->toInt();
  } else if (String(data->feedName()).equalsIgnoreCase("jessica")) {
    blue = data->toInt();
  } else if (String(data->feedName()).equalsIgnoreCase("emily")) {
    red = data->toInt();
  }

  Serial.print("received <- ");
  Serial.print(data->toInt());
  Serial.print(" from ");
  Serial.println(data->feedName());
    
  //change NeoPixel color here using format strip.Color(R,G,B)
  for(int i=0; i<strip.numPixels(); i++) { //turn off all NeoPixels
    strip.setPixelColor(i, strip.Color(red,green,blue));
  }
  strip.show(); //always remember to call strip.show() to display changes
}

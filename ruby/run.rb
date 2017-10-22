#!/usr/bin/env ruby

require 'rubygems'
require 'bundler/setup'
require 'mqtt'
require 'twitter'
require 'dotenv/load'

FEED_BASE = "#{ENV['MQTT_USERNAME']}/feeds/"

mqtt = MQTT::Client.connect(
  host:     ENV['MQTT_HOST'],
  port:     ENV['MQTT_PORT'],
  username: ENV['MQTT_USERNAME'],
  password: ENV['MQTT_PASSWORD'],
)

twitter = Twitter::REST::Client.new do |config|
  config.consumer_key        = ENV['TWITTER_CONSUMER_KEY']
  config.consumer_secret     = ENV['TWITTER_CONSUMER_SECRET']
  config.access_token        = ENV['TWITTER_ACCESS_TOKEN']
  config.access_token_secret = ENV['TWITTER_ACCESS_TOKEN_SECRET']
end

jokes = [
  'I got myself a new robot puppy. Dogmatic.',
  'Why do robots have summer holidays? To recharge their batteries.',
  'What is a robot’s favourite genre of music? Heavy metal.',
  'Why did the robot go back to school? He was getting a bit rusty.',
  'What sort of robot turns into a tractor? A transfarmer.',
  'Why did the robot cross the road? Because he was programmed to do it.',
  'What does R2D2 use to open PDF files? Adobe Wan Kenobi.',
  'I bought a dalek egg timer recently. After three minutes, it shouts “Eggs Terminate!”',
  'Why did the robot marry his fiancée? He couldn’t resistor.',
  'Why was the robot bankrupt? He had used all his cache.',
  'Why is a robot mechanic never lonely? Because he’s always making new friends.',
  'What do you call a pirate robot? Arrrrr2D2.'
]

puts "Hit ^C to terminate"

begin
  topic   = FEED_BASE + 'debug'
  payload = jokes.sample

  mqtt.publish(topic, payload)
  twitter.update(payload)
  puts payload
rescue SignalException => e
  puts "received Exception #{e}"
end

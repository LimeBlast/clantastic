#!/usr/bin/env ruby

require 'rubygems'
require 'bundler/setup'
require 'mqtt'
require 'dotenv/load'

FEED_BASE = "#{ENV['MQTT_USERNAME']}/feeds/"

client = MQTT::Client.connect(
  host:     ENV['MQTT_HOST'],
  port:     ENV['MQTT_PORT'],
  username: ENV['MQTT_USERNAME'],
  password: ENV['MQTT_PASSWORD'],
)

puts "Hit ^C to terminate"

begin
  loop do
    topic   = FEED_BASE + 'debug'
    payload = "At the third stroke, the time will be #{Time.now}"

    client.publish(topic, payload)
    puts payload
    sleep 5
  end
rescue SignalException => e
  puts "received Exception #{e}"
end

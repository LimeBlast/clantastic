#!/usr/bin/env ruby

require 'aylien_text_api'
require 'dotenv/load'

AylienTextApi.configure do |config|
  config.app_id  = ENV['AYLIEN_APP_ID']
  config.app_key = ENV['AYLIEN_APP_KEY']
end

aylien_client = AylienTextApi::Client.new


puts "Hit ^C to terminate"

begin
  loop do
    string = gets.chomp

    sentiment = aylien_client.sentiment(text: string, mode: 'tweet')
    puts sentiment
  end
rescue SignalException => e
  puts "received Exception #{e}"
end

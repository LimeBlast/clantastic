#!/usr/bin/env ruby

require 'sad_panda'
require 'dotenv/load'

puts "Hit ^C to terminate"

begin
  loop do
    string    = gets.chomp
    sad_panda = SadPanda::Emotion.new(string)
    sad_panda.call
    puts sad_panda.scores
  end
rescue SignalException => e
  puts "received Exception #{e}"
end

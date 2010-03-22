#!/usr/bin/env ruby

SPECHELPER = File.join(File.dirname(__FILE__), "spechelper.m")
def octave_eval(str, type)
  puts "evalling (#{type}): #{str}"
  ret = `octave --quiet --eval 'source("#{SPECHELPER}"); spec_formatter(#{str}, "#{type}")'`
  puts "    = #{ret.strip}"
  ret
end


ARGV.each do |fn|
  str = File.read(fn)
  str.gsub!(%r{(// octave (\w+):)(.*?)\n(.*?\n)}) do |match|
    e = octave_eval($3, $2)

    [$1, $3, "\n", e, "\n"].join
  end
  File.open(fn, "w") do |f|
    f.write str
  end

end


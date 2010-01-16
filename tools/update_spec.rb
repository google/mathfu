

SPECHELPER = File.join(File.dirname(__FILE__), "spechelper.m")
def octave_eval(str)
  puts "evalling #{str}"
  `octave --quiet --eval 'source("#{SPECHELPER}"); spec_formatter(#{str})'`
end


str = File.read(ARGV[0])
str.gsub!(%r{(// octave(?: \w+)?:)(.*?)\n(.*?\n)}) do |match|
  e = octave_eval($2)

  [$1, $2, "\n", e, "\n"].join
end

File.open(ARGV[0], "w") do |f|
  f.write str
end

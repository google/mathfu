

SPECHELPER = File.join(File.dirname(__FILE__), "spechelper.m")
def octave_vec4(str)
  `octave --quiet --eval 'source("#{SPECHELPER}"); spec_vec4(#{str})'`
end


str = File.read(ARGV[0])
str.gsub!(%r{(// octave vec4:)(.*?)\n(.*?\n)}) do
  e = octave_vec4($2)

  [$1, $2, "\n", e, "\n"].join
end

File.open(ARGV[0], "w") do |f|
  f.write str
end

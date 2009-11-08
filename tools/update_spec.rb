

str = File.read(ARGV[0])

def octave_vec3(str)

  `octave --quiet --eval 'source("spechelper.m"); spec_vec3(#{str})'`
end

str.gsub!(%r{(// octave vec3:)(.*?)\n(.*?\n)}) do |match|
  e = octave_vec3($2)

  [$1, $2, "\n", e].join

end

File.open(ARGV[0], "w") do |f|
  f.write str
end

#!/usr/bin/env ruby
################################################
## Created as 635 Mar 17 19:39  leana-kuku.rb ##
## sha256 a50ba9fa477385547428318c1eb57a1d    ##
##        954b92f65bd035fa18a67da6d81aab16    ##
## Just a first quick draft, but it should    ##
## give you all combinasons. Since doing them ##
## everyday is what is required of them.      ##
## Asking yourself, if this is useful? That's ##
## not part of the education.                 ##
## TODO: skip easy, record mistakes, reask    ##
################################################
x = 9.times.map { _1 + 1 }
#=> [1, 2, 3, 4, 5, 6, 7, 8, 9]
y = 9.times.map { _1 + 1 }
#=> [1, 2, 3, 4, 5, 6, 7, 8, 9]
x.shuffle!
#=> [1, 8, 6, 2, 5, 4, 3, 7, 9]
y.shuffle!
#=> [7, 3, 2, 5, 9, 4, 1, 6, 8]
all = x.map { |aa| y.map { |bb| [aa,bb] }}

def give_next(all)
  left_count = all.sum(&:size)
  puts "#{left_count} are left."
  if left_count == 0
    puts "you are done"
    return 81
  end

  foo = nil
  loop do
    foo = all[rand*9].delete_at(rand*9)
    break if foo&.size == 2
  end
  end
  puts [foo.join(" * "), "=", foo.first * foo.last].join(" ") unless foo.nil?
end

while give_next(all).nil?
  puts "continue? [enter/CTRL-c]"
  _answer = $stdin.readline
end

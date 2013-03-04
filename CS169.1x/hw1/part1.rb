#For those just starting out, one suggested way to work on your code is to have a command window open and a text editor with this
#file loaded.  Make changes to this file and then run 'ruby part1.rb' at the command line, this will run your program.  Once you're
#satisfied with your work, save your file and upload it to the checker.


def palindrome?(str)
  s = String.new
  s = str.gsub(/\W/,'').downcase
  s == s.reverse
end

def count_words(str)
  h = Hash.new(0)
  str.downcase.scan(/\w+/).each { |s| h[s] += 1 }
  h
end


#the code below this line will test your functions.  You should remove everything below this line prior to submitting your file

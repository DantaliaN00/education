def combine_anagrams(words)
  arr = []
  words.each { |word| 
    w_arr = []
    words.each { |check| 
	  if check.downcase.split('').sort == word.downcase.split('').sort
	    w_arr << check
	  end
    }
    arr << w_arr
  }
  arr.uniq
end
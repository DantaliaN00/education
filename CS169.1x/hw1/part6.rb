class Numeric
  @@currencies = {'dollar' => 1.0, 'yen' => 0.013, 'euro' => 1.292, 'rupee' => 0.019}
  def method_missing(method_id)
    singular_currency = method_id.to_s.gsub( /s$/, '')
    if @@currencies.has_key?(singular_currency)
      self * @@currencies[singular_currency]
    else
      super
    end
  end
  
  def in(cur)
    singular_currency = cur.to_s.gsub( /s$/, '')
    if @@currencies.has_key?(singular_currency)
      self / @@currencies[singular_currency]
    end
  end
end

class String
  def palindrome?()
    s = String.new
    s = self.gsub(/\W/,'').downcase
    s == s.reverse
  end
end

module Enumerable
  def palindrome?()
	s = []
	s = self.to_a.reverse.map { |c| c.to_s.downcase }
	s == self.to_a.map { |c| c.to_s.downcase }
  end
end

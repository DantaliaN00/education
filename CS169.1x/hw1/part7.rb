class CartesianProduct
  include Enumerable
  
  def initialize(a1, a2)
   @all = []
   a1.each { |a| 
    a2.each { |b| 
	  @all << [a, b]
	}
   }
  end
  
  def each 
    @all.each { |a| yield a }
  end
end

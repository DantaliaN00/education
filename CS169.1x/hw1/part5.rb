class Class
  def attr_accessor_with_history(attr_name)
    attr_name = attr_name.to_s   # make sure it's a string
    attr_reader attr_name        # create the attribute's getter
    attr_reader attr_name+"_history" # create bar_history getter
    class_eval %Q{
      def #{attr_name}=(v)
        if @#{attr_name+"_history"}.nil? 
          @#{attr_name+"_history"} = [nil, v] 
        else 
          @#{attr_name+"_history"} << v
        end
      end
    }
  end
end
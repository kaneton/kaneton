#! /usr/bin/ruby

require 'yaml'
require 'erb'

class String
  def escape_single_quotes
    self.gsub(/'/, "\\\\'")
  end
end

$template_tree = %{
      <%  if (@e["testsuite"]) 
            @e["testsuite"].each do |n| %>
      <%=     depth(n) %>
      <%   end
	  	else %>
	<%= leaf() %>
      <% end %>
} 

class Html

  @@template = nil
  @@current_cat = ""

  def initialize(e, dir)
    @e = e
	@dir = dir
  end

  def print_result()
	case @e["result"]
		when /PASS/
			return 0
		else
			return 1
	end
  end

  def get_binding
    binding
  end

  def mycat(filename)
     return "" if ! fd = File.open(filename, "r")

     str = ""
     while s = fd.gets()
       str += s.escape_single_quotes()
     end
     return str
  end

  def leaf()
	test="['#{@@current_cat}']['#{@e["name"]}']"
	str  = "$trace#{test}=#{print_result()};\n"
	str += "$output#{test}='#{mycat(@dir+'/'+@e["result_path"] + "/out")}';\n"
	return str
  end

  def depth(node)
	@@current_cat = @e["name"]
    return ERB.new($template_tree).result(Html.new(node, @dir).get_binding)
  end

end

root = YAML::parse(File.open(ARGV[0]))

rtree = ERB.new($template_tree)
tree = rtree.result(Html.new(root.transform, ARGV[1]).get_binding)

puts "<?"
puts tree.gsub(/([	 ]*\n)+/m,"\n")
puts "?>"

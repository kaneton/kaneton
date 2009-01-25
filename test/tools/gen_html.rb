#! /usr/bin/ruby

require 'yaml'
require 'erb'

$template_tree = %{
  <div class="config-meta">
    <div class="<%=get_header_style()%>" onclick="toogleDisplayMode(this);">
      <table class="test-header-header"><tr>
      <td class="test-header-name"><%=@e["name"]%></td>
      <td class="test-header-result"><%=print_result()%></td>
      </tr></table>
    </div>
       <div class="config-content-leaf">
      <%  if (@e["testsuite"]) 
            @e["testsuite"].each do |n| %>
      <%=     depth(n) %>
      <%   end
         else %>
       <pre>
      <%=  leaf() %>
      <% end %>
      </pre>
       </div>
  </div>
} 

$template_file = %{
  <html>
    <head>
      <link rel="stylesheet" class="text/css" href="style.css" />
    </head>
    <script language="javascript">
    function toogleDisplayMode(elt)
    {
        var parent = elt;
        
        while (parent.className != 'config-content-tree' && parent.className != 'config-content-leaf')
        {
            parent = parent.nextSibling;
        }
        
        if (parent.style.display == 'block')
        {
            elt.src = 'images/triangle-closed.gif';
            parent.style.display = 'none';
        }
        else
        {
            elt.src = 'images/triangle-open.gif';
            parent.style.display = 'block';
        }        
    }
    </script>
    <body>
      <%= tree %>
    </body>
  </html>
}

class Html

  @@template = nil

  def initialize(e, dir)
    @e = e
	@dir = dir
  end

  def print_result()
    res = get_result(@e)
    return "" if (@e["coef"] && @e["coef"] == 0)
    if (res == 1)
      return "PASS"
    else
      return "FAIL (#{(res * 100).round()})"
    end
  end

  def get_result(node)
    res = 0
    i = 0;
    coef = 1;

    if (node["testsuite"]) 
      node["testsuite"].each do |n|
        coef = 1
        coef = n["coef"] if (n["coef"])
        res += get_result(n)
        i = i + coef 
      end
      res = (res / (i * 1.0));
    else
      case node["result"]
        when /PASS/
          return 1
        when /FAIL/
          return 0
        when /SKIP/
          return 0
        else
          return 0
      end
    end
    coef = 1
    coef = node["coef"] if (node["coef"])
    return (res * coef)
  end

  def get_binding
    binding
  end

  def get_header_style()
    if (@e["coef"] && @e["coef"] == 0)
      return "test-header-none"
    end
    case print_result()
      when /PASS/
        return "test-header-pass"
      when /FAIL/
        return "test-header-fail"
      else
        return "test-header-none"
    end
  end

  def mycat(filename)
     return "" if ! fd = File.open(filename, "r")

     str = ""
     while s = fd.gets()
       str += s
     end
     return str
  end

  def leaf()
    return "" if ! @e["result_path"]
    str = "\n"
    str += "<b>out:</b>\n"
    str += mycat(@dir+'/'+@e["result_path"] + "/out")
    str += "<b>err:</b>\n"
    str += mycat(@dir+'/'+@e["result_path"] + "/err")
    return str
  end

  def depth(node)
    return ERB.new($template_tree).result(Html.new(node, @dir).get_binding)
  end

end

root = YAML::parse(File.open(ARGV[0]))

rtree = ERB.new($template_tree)
tree = rtree.result(Html.new(root.transform, ARGV[1]).get_binding)

rhtml = ERB.new($template_file)
html = rhtml.run

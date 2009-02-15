#! /usr/bin/ruby

require 'ftools'
require 'yaml'

def mymkdir(dir)
  begin
    Dir.mkdir(dir)
  rescue
  end
end

class Check

  def initialize(tarball)
    @filename = tarball.filename()
    @tarball = tarball
  end
  
  def pre(e, result)
    puts "Execute testsuite #{e["name"]}"
  end

  def post(e, result)
  end

  def cmd_env(e)
	  str=""
	  for k in e.keys do
	    str += "TEST_#{k.upcase}='#{e[k]}' "
	  end
	  str
  end

  def leaf(e, result)
    print "Execute test #{e["name"]} "
    STDOUT.flush
    
    if (e["skip"])
      puts e["result"] = "SKIP"
      return true 
    end

    e["result"] = "IN PROGRESS ..."
    
    File.open('res.yml', 'w') { |f| f.write(@tarball.testsuite.to_yaml) }

	cmd  = cmd_env(e) + " "
	cmd += "test/#{e["exec"]} " # command
	cmd += " > #{result}/out 2> #{result}/err; " # rediction for err and out
	cmd += "/bin/echo -n $? > #{result}/ret; " # put the error code into a file
	cmd += "cat #{result}/ret" # print the error code on stdout to get the restul into ret
    ret=`#{cmd}`

    e["result_path"] = result 

    if (ret == "137")
      puts e["result"] = "TIMEOUT"
      return !e["fatal"]
      r = 0
    elsif (ret == "0")
      puts e["result"] = "PASS"
    else
      puts e["result"] = "FAIL"
      return !e["fatal"]
    end
    return true
  end
end

class Tarball

  attr_reader :filename, :testsuite

  def initialize(file, testsuite)
    @filename = file
    @testsuite = testsuite
  end

  def run(obj)
	mymkdir("result")
    depth(obj, @testsuite, "result")
  end

  def depth(obj, tree, result)
      if (tree["testsuite"])
        obj.pre(tree, result) 
        tree["testsuite"].each do |e|
            mymkdir("#{result}/#{e["name"]}")
            depth(obj, e, "#{result}/#{e["name"]}")
        end
        obj.post(tree, result)
      else
        return if (! obj.leaf(tree, result))
      end
  end
end

root = YAML::parse(File.open(ARGV[0]))
testsuite = root.transform

t = Tarball.new(ARGV[1], testsuite)
t.run(Check.new(t))

File.open("res.yml", 'w') { |f| f.write(testsuite.to_yaml) }

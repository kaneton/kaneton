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
	@dir = Dir.pwd + '/' + @tarball.dir
  end
  
  def pre(e, result)
    puts "Execute testsuite #{e["name"]}"
  end

  def post(e, result)
  end

  def cmd_env(e)
	  str=""
	  str += "TEST_DIR='#{@dir}' "
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
    
    File.open('res.yaml', 'w') { |f| f.write(@tarball.testsuite.to_yaml) }

#cmd="timeout #{e["timeout"]} #{cmd}" if (e["timeout"])

	cmd  = "cd #{@dir}/root && " # execute the command in the root directory
	cmd += cmd_env(e) + " "
	cmd += "#{@dir}/../#{e["exec"]} " # command
	cmd += " > #{@dir}/#{result}/out 2> #{@dir}/#{result}/err; " # rediction for err and out
	cmd += "/bin/echo -n $? > #{@dir}/#{result}/ret; " # put the error code into a file
	cmd += "cat #{@dir}/#{result}/ret" # print the error code on stdout to get the restul into ret
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

  attr_reader :filename, :testsuite, :dir

  def initialize(file, testsuite)
    @dir = file.sub(/.tar.bz2/, "")
    @filename = file
    @testsuite = testsuite

    mymkdir("#{@dir}")
    mymkdir("#{@dir}/result")
    mymkdir("#{@dir}/root")
  end

  def run(obj)
    depth(obj, @testsuite, "result")
  end

  def depth(obj, tree, result)
      if (tree["testsuite"])
        obj.pre(tree, result) 
        tree["testsuite"].each do |e|
            mymkdir("#{@dir}/#{result}/#{e["name"]}")
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

File.open(ARGV[2], 'w') { |f| f.write(testsuite.to_yaml) }

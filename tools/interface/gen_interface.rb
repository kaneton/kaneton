#!/usr/bin/ruby

require 'optparse'
require 'fileutils'
require 'yaml'

$opt = OptionParser.new;
$opt.on("-h", "--help", "print this usage") do
  puts $opt.to_s;
  exit(1);
end
$opt.banner += " <description file>";
$rest = $opt.parse(ARGV);

if $rest.size != 1 then
  puts "no input file";
  exit(1);
end

$input = $rest[0];

def header(f)
  f.puts "/*
 * project       kaneton
 *
 * generated by gen_interface.rb
 *
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file describes the core's interface.
 *
 * in other words, this file describes the services provided by the kernel
 * so the core functions a task is allowed to use.
 *
 * each time a message is destinated to the core, the corresponding
 * function of the interface is launched.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

"
end

def process(f, manager, func)
  operation = manager + "_" + func['operation']
  capability = manager.upcase + "_OPERATION_" + func['operation'].upcase

  f.puts "/*
 * this function launchs the #{operation}() function.
 */

"
  f.puts "t_error\t\tinterface_#{operation}(o_message*\tmessage)\n{\n"

  f.puts "  t_error\t\tresult;\n\n"

  f.puts "  if (capability_check(#{manager}, #{capability}) != ERROR_NONE)\n    return (ERROR_UNKNOWN);\n\n"

  args = "";
  i = 0
  func['argument'].each do | arg |
    if i != 0 then
      args += ",\n\t\t\t"
    end
    args += "message->u.#{operation}.arg#{i}"
    i += 1
  end

  f.puts "  result = #{operation}(#{args});\n\n"

  f.puts "  return (ERROR_NONE);\n}\n\n"
end

File.open("interface.c", "w") do | f |
  begin

    header(f)

    data = YAML.load_file($input)

    data.each do | section |
      manager = section["manager"]
      section["interface"].each do | func |
        process(f, manager, func)
      end
    end
  rescue
    $stderr.puts $!
    exit(1)
  end
end


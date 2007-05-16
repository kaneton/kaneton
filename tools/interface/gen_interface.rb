#!/usr/bin/ruby

require 'optparse'
require 'fileutils'
require 'yaml'
load 'kinterface.rb'

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
$nsyscall = 0

def process(kinterface, manager, func)
  operation = manager + "_" + func['operation']
  capability = manager.upcase + "_OPERATION_" + func['operation'].upcase

  kinterface.puts "/*
 * this function launchs the #{operation}() function.
 */

"
  kinterface.puts "t_error\t\tinterface_#{operation}(o_message*\tmessage)\n{\n"

  kinterface.puts "  t_error\terror;\n"

  code_cap = "  if (capability_check(#{manager}, #{capability}) != ERROR_NONE)\n    return (ERROR_UNKNOWN);\n\n"

  args = ""
  code_decl = ""
  code_serialize = ""
  j = i = 1
  cap = false
  func['argument'].each do | arg |
    if i != 1 || cap then
      args += ",\n\t\t\t"
    end

    if arg.index("(out) ") == 0 then
      type = arg[6..arg.length]

      dbl_ptr = type.index("**") != nil
      ptr = type.index("*") != nil

      type = type.sub('*', '')

      code_decl += "  #{type}\tresult#{j};\n"

      if dbl_ptr then
        code_serialize += "  message->u.reply.u.#{operation}.result#{j} = *result#{j};\n"
      else
        code_serialize += "  message->u.reply.u.#{operation}.result#{j} = result#{j};\n"
      end


      args += "&result#{j}"

      j += 1
    else
      if arg.index("(capability)") == 0 then
        args += "message->u.request.capability.object"
        cap = true
      else
        args += "message->u.request.u.#{operation}.arg#{i}"
        i += 1
      end
    end
  end

  kinterface.puts code_decl + "\n"

  kinterface.puts code_cap

  kinterface.puts "  error = #{operation}(#{args});\n\n"

  kinterface.puts "  message->u.reply.error = error;\n"

  kinterface.puts code_serialize + "\n"

  kinterface.puts "  return (ERROR_NONE);\n}\n\n"

  return "  interface_#{operation},\n"
end

File.open("interface.c", "w") do | kinterface |
  begin

    header_kinterface(kinterface)

    data = YAML.load_file($input)

    dispatch = ""
    data.each do | section |
      manager = section["manager"]
      section["interface"].each do | func |
        dispatch += process(kinterface, manager, func)
      end
    end

    kinterface.puts "/*
 * this structure dispatchs incoming system calls.
 */

t_interface_dispatch dispatch[] =\n{\n#{dispatch}};\n\n"

    footer_kinterface(kinterface)
  rescue
    $stderr.puts $!
    exit(1)
  end
end


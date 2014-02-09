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
$nsyscalls = 0
$dispatch = ""
$macros = ""
$type_request = ""
$type_reply = ""

def process(kinterface, hinterface, uinterface, manager, func)
  operation = manager + "_" + func['operation']

  if operation.index("capability_") != nil then
    puts "ignoring unsupported operation #{operation}"
    return
  end

  if func['operation'].index("attribute_") == 0 then
    chiche = true
  else
    chiche = false
  end
  if func['capability'] == nil then
    capability = manager.upcase + "_OPERATION_" + func['operation'].upcase
  else
    capability = manager.upcase + "_OPERATION_" + func['capability']
  end

  if !chiche then
    kinterface.puts "/*
 * this function launchs the #{operation}() function.
 */

"
    kinterface.puts "t_status\t\tinterface_#{operation}(o_syscall*\tmessage)\n{\n"
    kinterface.puts "  t_status\terror;\n"
  end

  code_cap = "  if (capability_check(#{manager}, #{capability}) != STATUS_OK)\n    return (STATUS_UNKNOWN_ERROR);\n\n"

  trequest = ""
  treply = ""
  args = ""
  uargs = ""
  code_decl = ""
  code_serialize = ""
  code_output = ""
  message = "  message.id = 0;

  message.u.request.operation = INTERFACE_#{manager.upcase}_#{func['operation'].upcase};\n"
  j = i = 1
  cap = false
  func['argument'].each do | arg |
    if i != 1 || j != 1 || cap then
      args += ",\n\t\t\t"
      uargs += ",\n\t\t\t"
    end

    type = arg
    dbl_ptr = type.index("**") != nil
    ptr = type.index("*") != nil
    original = type
    type = type.sub('*', '')

    if arg.index("(out) ") == 0 then
      type = type[6..type.length]

      code_decl += "  #{type}\tresult#{j};\n"

      if dbl_ptr then
        code_serialize += "  message->u.reply.u.#{operation}.result#{j} = *result#{j};\n"
      else
        # FIXME: can be optimised
        code_serialize += "  message->u.reply.u.#{operation}.result#{j} = result#{j};\n"
      end

      treply += "\n	  #{type.gsub('*', '')}\tresult#{j};"
      args += "&result#{j}"
      if dbl_ptr then
        uargs += "#{type}\t\tresult#{j}"
      else
        uargs += "#{original[6..original.length]}\t\tresult#{j}"
      end
      code_output += "  *result#{j} = message.u.reply.u.#{operation}.result#{j};\n"

      j += 1
    else
      if arg.index("(capability)") == 0 then

        #args += "message->u.request.capability.object"
        #cap = true

        args += "message->u.request.u.#{operation}.arg#{i}"
        trequest += "\n	  t_id\targ#{i};"
        uargs += "t_id\t\targ#{i}"
        message += "  message.u.request.u.#{operation}.arg#{i} = arg#{i};\n"
        i += 1

      else
        if arg.index("(var) ") == 0 then
          type = type[6..type.length]
          # FIXME: can be optimised
          code_serialize += "  message->u.reply.u.#{operation}.result#{j} = message->u.request.u.#{operation}.arg#{i};\n"
          treply += "\n	  #{type}\tresult#{j};"
          j += 1
          # FIXME: uinterface
        end
        trequest += "\n	  #{type}\targ#{i};"
        if ptr then
          args += "&message->u.request.u.#{operation}.arg#{i}"
          message += "  message.u.request.u.#{operation}.arg#{i} = *arg#{i};\n"
        else
          args += "message->u.request.u.#{operation}.arg#{i}"
          message += "  message.u.request.u.#{operation}.arg#{i} = arg#{i};\n"
        end
        uargs += "#{original}\t\targ#{i}"
        i += 1
      end
    end
  end

  if !chiche then
    kinterface.puts code_decl + "\n"
    #kinterface.puts code_cap # FIXME
    kinterface.puts "  error = #{operation}(#{args});\n\n"
    kinterface.puts "  message->u.reply.error = error;\n"
    kinterface.puts code_serialize + "\n"
    kinterface.puts "  return (STATUS_OK);\n}\n\n"
  end

  uinterface.puts "t_status\t\t#{operation}(#{uargs})\n{\n  o_syscall\t\tmessage;\n  i_node\t\tnode;\n  t_vsize\t\tsize;\n\n"

  uinterface.puts "  node.machine = 0;
  node.task = 0;

" # XXX this is bad

  uinterface.puts message + "\n"

  uinterface.puts "  message_send(node, MESSAGE_TYPE_INTERFACE, (t_vaddr)&message, sizeof (message));

  if (message_receive(MESSAGE_TYPE_INTERFACE, (t_vaddr)&message, &size, &node) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

"

  uinterface.puts "  if (message.u.reply.error != STATUS_OK)\n    return (STATUS_UNKNOWN_ERROR);\n\n"

  uinterface.puts code_output + "\n"

  uinterface.puts "  return (STATUS_OK);\n}\n\n"


  $type_request += "\tstruct\n\t{#{trequest}\n\t}\t\t#{operation};\n" if trequest != ""
  $type_reply += "\tstruct\n\t{#{treply}\n\t}\t\t#{operation};\n" if treply != ""

  $dispatch += "  interface_#{operation},\n"
  $macros += "#define INTERFACE_#{manager.upcase}_#{func['operation'].upcase} #{$nsyscalls}\n"
  $nsyscalls += 1
end

def process_attribute(kinterface, hinterface, uinterface, manager, attr)
  name = attr['name'];
  type = attr['type'];

  if manager.index("region") != nil then
    puts "ignoring unsupported object region"
    return
  end

  if type.index("[]") != nil then
    puts "ignoring unsupported attribute #{name}"
    return
  end

  func = {
    'operation' => "attribute_#{name}",
    'argument' => ['(capability)', "(out) #{type}*"]
  }

  kinterface.puts "/*
 * this function get the #{name} attribute of the o_#{manager} object.
 */

"

  kinterface.puts "t_status\t\tinterface_#{manager}_attribute_#{name}(o_syscall*\tmessage)\n"

  kinterface.puts "{\n  o_#{manager}*\t\to;\n\n"

  # XXX cap
  kinterface.puts "  if (#{manager}_get(message->u.request.u.#{manager}_attribute_#{name}.arg1, &o) != STATUS_OK)\n"
  kinterface.puts "    {\n"
  kinterface.puts "      message->u.reply.error = STATUS_UNKNOWN_ERROR;\n"
  kinterface.puts "    }\n"
  kinterface.puts "  else\n"
  kinterface.puts "    {\n"
  kinterface.puts "      message->u.reply.error = STATUS_OK;\n"
  kinterface.puts "      message->u.reply.u.#{manager}_attribute_#{name}.result1 = o->#{name};\n"
  kinterface.puts "    }\n"
  kinterface.puts "\n  return (STATUS_OK);"
  kinterface.puts "}\n\n"

  process(kinterface, hinterface, uinterface, manager, func);
end

kinterface = File.open("interface.c", "w")
hinterface = File.open("interface.h", "w")
uinterface = File.open("libkaneton.c", "w")
begin

  header_kinterface(kinterface)
  header_hinterface(hinterface)
  header_uinterface(uinterface)

  data = YAML.load_file($input)

  data.each do | section |
    manager = section["manager"]
    section["interface"].each do | func |
      process(kinterface, hinterface, uinterface, manager, func)
    end
    if section["attribute"] != nil then
      section["attribute"].each do | attr |
        process_attribute(kinterface, hinterface, uinterface, manager, attr)
      end
    end
  end

  kinterface.puts "/*
 * this structure dispatchs incoming system calls.
 */

t_interface_dispatch dispatch[] =\n{\n#{$dispatch}};\n\n"

  hinterface.puts "/*
 * ---------- macros ----------------------------------------------------------
 */

#{$macros}

#define INTERFACE_NSYSCALLS #{$nsyscalls}\n\n"

  hinterface.puts "/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_id			id;
  union
  {
    struct
    {
      t_operations	operation;
      t_capability	capability;
      union
      {
#{$type_request}      }	u;
    } request;
    struct
    {
      t_status		error;
      union
      {
#{$type_reply}      } u;
    } reply;
  } u;
}			o_syscall;

typedef t_status (*t_interface_dispatch)(o_syscall*);

"

  footer_kinterface(kinterface)
  footer_hinterface(hinterface)
  footer_uinterface(uinterface)
rescue
  $stderr.puts $!
  exit(1)
end

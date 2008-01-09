'''
* SUMMARY

Cuts C code from text files, delimited by markup.
Also supports function cutting, see FunctionExtractor coding style limitations.

* SYNTAX

* USAGE
- only_strip_markup
  False to do a complete code & function cut
  True just to strip cut related markup (no code cut takes place)

* AUTHORS

Elie BLETON (elie@epita.fr)
Legacy: buchman (FIXME: bucsome legacy from

'''

import pprint
import math
import re
import types
import FunctionExtracter

class CodeCutter:
    def __init__(self, file_handle, only_strip_markup):
        if (not (type(file_handle) is file)):
            raise TypeError, 'Parameter is expected to be a file handle'

        self.function_extracter = FunctionExtracter.FunctionExtracter(file_handle)
        self.compile_regexps()
        self.init_brouteur_state()

        content = ""
        file_handle.seek(0)
        for line in file_handle.readlines():
            content += line

        content = self.cut_functions(content, only_strip_markup)
        content = self.cut_code(content, only_strip_markup)
        self.result = content

    def set_lower_cut_boundary(self, low_boundary):
        # FIXME: Check is_digit(low_boundary)
        self.low_boundary = low_boundary

    def set_higher_cut_boundary(self, high_boundary):
        # FIXME: Check is_digit(high_boundary)
        self.high_boundary = hight_boundary

    def init_brouteur_state(self):
        self.low_boundary = 0
        self.high_boundary = 42

    def compile_regexps(self):
        self.re_markup = re.compile("/\* (\[/?cut[^\]]*)] \*/")
        self.re_markup_start = re.compile("\[/?cut")
        self.re_fxmarkup = re.compile("/\*[^/[]*(\[fxcut[^\]]*)\][^/]*\*/")

        self.re_simple_fxmarkup = re.compile("\[fxcut[^\]]*\]")

    def parse_cut_params(self, args_string):
        if (type(args_string) != types.StringType):
            raise "Expected a string, got a " + str(type(args_string))

        replacement = ""
        stage = -1
        no_smart_stuff_left = False

        for param in re.findall("([^ ]+)", args_string):
            if no_smart_stuff_left:
                replacement += param + " "
                continue

            kstage = re.match("k([0-9])", param)
            if (kstage) != None:
                stage = kstage.group(1)
            elif param == "fixme:":
                replacement = "// FIXME: "
                no_smart_stuff_left = True
            elif param == "code:":
                replacement = "// FIXME\n\t"
                no_smart_stuff_left = True

        return [replacement, stage]

    def cut_code(self, content, strip_only_markup = False):
        if (type(content) != types.StringType):
            raise "Expected a string, got a " + str(type(content))
        '''Cut file contents delimited by markup
        Returns cutted content'''
        cutting = False
        replacement = ""
        result = ""

        for block in self.re_markup.split(content):
            if not self.re_markup_start.match(block):
                if strip_only_markup or (not cutting):
                    result += block
            else:
                # cut block interpretation
                if (block[1] == "/"):
                    cutting = False
                    result += replacement
                    replacement = ""
                elif not strip_only_markup:
                    params = self.parse_cut_params(block[4:])
                    cut_stage = int(params[1])

                    if (cut_stage == -1) or \
                    ((cut_stage >= self.low_boundary) and \
                     (cut_stage <= self.high_boundary)):
                        replacement = params[0]
                        cutting = True
                else:
                    cutting = False
        return (result)

    def cut_functions(self, content, strip_only_markup = False):
        if (strip_only_markup):
            return (self.re_simple_fxmarkup.sub("", content))

        # Builds a tuple of MatchObjects for [fxcut] markups
        cutlist = []
        searchpos = 0
        fxcut = self.re_fxmarkup.search(content, searchpos)

        while (fxcut != None):
            searchpos = fxcut.end()
            startline = self.position_to_line(content, fxcut.start())
            endline = self.position_to_line(content, fxcut.end())

            cutlist.append([startline, endline, self.nearest_function_block(endline), self.parse_cut_params(fxcut.groups(0)[0])])
            fxcut = self.re_fxmarkup.search(content, searchpos)

        print "Cutlist:"
        pprint.pprint(cutlist)

        # Initializes the cutting subsystem
        cutting = False
        result = ""
        lineno = 0
        cutblock_counter = 0

        # Cut !
        for line in content.splitlines(True):
            lineno += 1

            # No more cuts ?
            if (cutblock_counter == len(cutlist)):
                result += line
                continue

            # Comment cut (silent)
            if (lineno >= cutlist[cutblock_counter][0]):
                cutting = True
            if (lineno > cutlist[cutblock_counter][1]):
                cutting = False

            # Function body cut (usual options apply
            cut_stage = int(cutlist[cutblock_counter][3][1])
            if (lineno > cutlist[cutblock_counter][2][1]):
                if (cut_stage == -1) or \
                        ((cut_stage >= self.low_boundary) and \
                             (cut_stage <= self.high_boundary)):
                    cutting = True

            if (lineno >= cutlist[cutblock_counter][2][2]):
                result += "\t" + cutlist[cutblock_counter][3][0] + "\n"
                cutting = False
                cutblock_counter += 1

            # Line cut ?
            if (cutting == False):
                result += line

        return (result)

    def position_to_line(self, content, position):
        lineno = 1
        charno = 0
        for line in content.splitlines(True):
            lenline = len(line)
            if ((position >= charno) and (position < (charno + lenline))):
                return (lineno)
            lineno += 1
            charno += lenline

    def nearest_function_block(self, lineno):
        nearest_funcblock = False
        nearest_distance = 1000000000 # there's no int.max in python ??

        for funcblock in self.function_extracter.functions:
            '''funcblock[0] is the line at which the function declaration starts'''
            dist = math.fabs(funcblock[0] - lineno)
            if (dist < nearest_distance):
                nearest_funcblock = funcblock
                nearest_distance = dist

        return nearest_funcblock

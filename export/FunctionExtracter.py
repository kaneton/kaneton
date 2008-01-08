'''
* SUMMARY

Load all function declarations from a file handle

* USAGE

Instanciate with a file handle
Retrieve the public variable functions
This won't close the file handle, so take care

   `functions` public variable reference

   [[(int) DEFL, (int) BODY_SLINE, (int) BODY_ELINE, (string) FUNC_HEADER]]

   Array entries:
   - DEFL: Line where the function declaration starts
   - BODY_SLINE: Line where the function body starts
   - BODY_ELINE: Line where the function body ends
   - FUNC_HEADER: The function declaration, as text

* AUTHORS

Parts of this code were taken from mkp.py by pwipwi@lse.epita.fr
Rest of it: elie@epita.fr

'''

import re

class FunctionExtracter:
    def __init__(self, file_handle):
        if (not (type(file_handle) is file)):
            raise TypeError, 'Parameter is expected to be a file handle'
        self.compile_regexps()
        self.init_brouteur_state()
        self.broute_file(file_handle)

    def compile_regexps(self):
        self.re_function_definition = re.compile('^[a-zA-Z0-9_](([\*\w]*)\s+\**)+\w+\(') # made by pwipwi
        self.re_comment_start = re.compile('/\*') # comment start
        self.re_comment_end = re.compile('\*/') # comment end

    def init_brouteur_state(self):
        self.comment_counter = 0
        self.scope_counter = 0
        self.lineno = 0
        self.functions = []
        self.init_function_brouteur_state()

    def init_function_brouteur_state(self):
        self.inside_function_header = False
        self.definition_lineno = -1
        self.body_start_lineno = -1
        self.body_end_lineno = -1
        self.definition_list = []

    def broute_file(self, file_handle):
        ''' Line Broutage - Because it isn't line parsing '''

        file_handle.seek(0)
        for line in file_handle.readlines():
            self.lineno += 1
            self.broute_line(line)

    def broute_line(self, line):
        ''' Perform various broutage on a text line'''

        # Unreliable comment handling
        if self.re_comment_start.search(line): self.comment_counter += 1
        if self.re_comment_end.search(line): self.comment_counter -= 1

        self.broute_line_function_declaration(line)
        self.broute_line_scope(line)

        # Scope found
        if ((self.body_start_lineno != -1) and (self.body_end_lineno != -1)):
            self.functions.append([self.definition_lineno, self.body_start_lineno, self.body_end_lineno, "".join(self.definition_list)])
            if (self.scope_counter != 0): raise Exception, "Unexpected scope value"

            # Reset internal state
            self.init_function_brouteur_state()

    def broute_line_function_declaration(self, line):
        ''' `Eats` a function declaration '''

        # Function definition start
        if self.re_function_definition.search(line) and (self.comment_counter == 0):
            self.inside_function_header = True
            self.definition_lineno = self.lineno

        # Function definition broutage
        if self.inside_function_header:
            if not re.search('\)\n', line):
                # Inside the definition
                self.definition_list.append(line)
            else:
                # End of definition
                line = re.match("(.*)$", line).group(0)
                line = line + ';\n'
                self.definition_list.append(line)
                self.inside_function_header = False

    def broute_line_scope(self, line):
        ''' Actualizes the scope counter. A C scope is a zone delimited by '{' and '}' '''

        scope_start_pos = 0
        scope_end_pos = 0
        while (True):
            scope_start_pos = line.find("{", scope_start_pos)
            scope_end_pos = line.find("}", scope_end_pos)

            if ((scope_start_pos != -1) and (scope_end_pos == -1)):
                self.scope_counter += 1
                if (self.scope_counter == 1): self.body_start_lineno = self.lineno
            elif ((scope_start_pos == -1) and (scope_end_pos != -1)):
                self.scope_counter -= 1
                if (self.scope_counter == 0): self.body_end_lineno = self.lineno
            elif ((scope_start_pos == -1) and (scope_end_pos == -1)):
                break

            scope_start_pos += 1
            scope_end_pos += 1

# HOW TO TEST
# Copy the following script in the same directory as this file

'''import sys
import pprint
import FunctionExtracter

f = open("region.c", "r")
ex = FunctionExtracter.FunctionExtracter(f)
pprint.pprint(ex.functions)
f.close()
sys.exit(42)'''

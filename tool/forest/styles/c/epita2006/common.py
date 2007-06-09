##
## common.py for forest in /home/nfs/acu2007/eymard_c
##
## Made by Christophe Eymard
## Login   <eymard_c@epita.fr>
##
## Started on  Fri Oct  6 14:57:43 2006 Christophe Eymard
## Last update Wed Nov  8 20:52:51 2006 Christophe Eymard
##

import re
import errors

s_correct_id = "^[^\w_]*[a-z][a-z0-9_]*$"
e_correct_id = re.compile(s_correct_id)

def check_id_validity(e, error, id):
    if not e_correct_id.search(id):     # incorrect id
        errors.add(e, error, errors.ERRORS_STYLE,
                   "the id \'" + id + "\' is incorrectly formatted\n")

s_correct_enumf = "[A-Z][A-Z0-9_]*$"
e_correct_enumf = re.compile(s_correct_enumf)

def check_enum_field_validity(e, error, enumf):
    if not e_correct_enumf.search(enumf):
        errors.add(e, error, errors.ERRORS_STYLE,
                   "the enumeration field \'" + enumf + "\' is incorrectly formatted\n")

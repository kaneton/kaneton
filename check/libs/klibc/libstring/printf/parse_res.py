#!/usr/bin/python
"""
This function parse result of printf test
and return parsed_result
parsed_result is used in check.py->check_result functions
"""

def	parse_result(result):
	print "printf_parsing"
	parsed_result = result
	return parsed_result[1]

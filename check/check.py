#!/usr/bin/python
"""
check.py
This program is for testing kaneton
"""


__author__	= "jacob_s@lse.epitech.net"
__version__ 	= "0.0.1"
__date__ 	= ""

def 	check_result(result_file, result_2_check):
	"""
	This function simply check if parsed result and
	result file are the same
	"""

	tmp = result_file.split('/')
	result = OpenFile(result_file + tmp[-2]  +  ".res")
	print "result: " + result
	print "result2check: " + result_2_check

	if result == result_2_check:
		return 1

def	get_function_addr(cmd):
 	"""
	Return addr of function named cmd in kaneton
	"""

	obj = os.popen("nm ../core/kaneton/kaneton | grep " + cmd +  " | cut -d ' ' -f 1")
	addr = obj.read()
	addr = addr.strip("\n")
	return addr

def 	SendCommand(cmd):
	"""
	Send Command to execute on kaneton and receive the result, for sending
	it to the result checker
	"""

	result_buff = []
	result	= []

	data = ("command")
	tosend = (data)
	tosend_size = 8
	serial_send(tosend, tosend_size)

	cmd = get_function_addr(cmd)
	tosend = (cmd)
	tosend_size = (len(cmd) + 1)
	serial_send(tosend, tosend_size)

	tmp = serial_recv()
	result = [0,""]

	while tmp[1] == "printf":
		tmp = serial_recv()
		result[0] += tmp[0]
		result[1] += tmp[1]
		tmp = serial_recv()

	return result

def	OpenFile(file_path, p = 1):
	"""
	Openfile and return its content
	"""
	try:
		testfile = open(file_path, "rb")
	except IOError:
		if p == 1:
			print "The file " + file_path + " doesn't exist"
		return  0
	file_content = testfile.read()
	testfile.close()
	return file_content

def	get_path(test_list):
	"""
	Add to test_list all subdir dir with a 'list' file.
	Return the numbre of 'list' file found
	"""
	i = 0
	nb = 0

	while i < len(test_list):
		new_list = OpenFile(test_list[i] + "list", 0)
		if new_list == 0:
			i += 1
		else:
			new_list = new_list.split()
			nb = len(new_list)
			c = 0
			while c < nb:
				test_list.insert(i + c  + 1, test_list[i] + new_list[c] + '/')
				c += 1
			test_list.remove(test_list[i])
			i += nb
	return	nb


def	get_functions_name(test_list):
	"""
	Get path name and after it function name in the list
	"""

	i = 0

	while i < len(test_list):
		tmp_list = test_list[i].split('/')
		test_list.insert(i + 1,["function", "check_" +  tmp_list[-3] + '_' + tmp_list[-2]])
		i += 2


	return

def	get_file_list(test_list):
	"""
	list all file in modules/ to load
	"""
	i = 0
	while i < len(test_list):
		if type(test_list[i]) == type(""):
			tmp = test_list[i].split('/')
			path = "/".join(tmp[0:-2]) + "/modules/"
			if os.path.isdir(path):
				try:
					test_list.index(["loadfile", path])
				except ValueError:
					test_list.insert(i,["loadfile", path])
		i += 1

	return test_list

def	ListTest():
	"""
	ListTest : get Dir/testlist and scan testlist for all test
	checksubdir and list sub-test
	"""
	i = 0
	test_list = [""]
	nb_path_found = 1

	while nb_path_found:
		nb_path_found = get_path(test_list)

	get_functions_name(test_list)
	get_file_list(test_list)

	return test_list

def	set_and_parse_result(test_list, result):
	"""
	Add current path to path list and reload parse file
	Parse result
	Return parsed result
	"""
	tmp = test_list.split('/')
	sys.path[-1] = test_list.rstrip(tmp[-2]);
	print "in " + sys.path[-1]
	import parse_res
	reload(parse_res)
	parsed_result = parse_res.parse_result(result)
	return parsed_result

def	load_file(test_list):
	file_list =  os.listdir(test_list)

	i = 0

	while i < len(file_list):
		objs = os.popen("base64 -e " + test_list + file_list[i] + " tmp")
		file_content = OpenFile("tmp", 0)
		#delete tmp
		if file_content > 0:
			print "loading file: " + test_list + file_list[i]
			data = ("loadfile")
			tosend = (data)
			tosend_size = 8
			serial_send(tosend, tosend_size)
			serial_send(file_list[i], len(file_list[i]))

			serial_send(file_content, len(file_content))

		i += 1


if __name__ == "__main__":
	from kserial import *
	from struct  import *
	from re      import *
	import os, array, sys

	i = 0			#on peut donner l'index de demarrage pour qu il se relance ou sa couille
				#il faut donc avant l afficher en cas de deco ou le sauvegarder
				#ne pas oublier de reload les files du test ds ce cas
				# if i != 0
				# loadfile de la section courrante
				# donc peut etre plus d indication
				# i num de seq de test et c num de test c[-1] == load file
				# if i != 0 && c[1].strip(' ') == "loadfile  " ...
	total_ok = 0
	total_failed = 0

	serial_init("/dev/pts/3")
	test_list = ListTest()


	while i < len(test_list):
		if test_list[i][0] == "loadfile":
			print "file loading ..."
			load_file(test_list[i][1])
			i += 1
		elif test_list[i][0] == "function":
			print "Launching test: " + test_list[i - 1]
			result = SendCommand(test_list[i][1])
			parsed_result = set_and_parse_result(test_list[i - 1], result)
			if check_result(test_list[i - 1], parsed_result):
				print "[OK]"
				total_ok += 1
			else:
				print "[FAILED]"
				total_failed += 1
			i += 1
		else:
			i += 1

	print "Passed: " + str(total_ok)
	print "Failed: " + str(total_failed)

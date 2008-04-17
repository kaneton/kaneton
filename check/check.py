#!/usr/bin/python
"""
check.py
This program is for testing kaneton
"""

__author__	= "jacob_s@lse.epitech.net, bucchi_m@lse.epita.fr, elie@lse.epita.fr"
__version__ 	= "0.3.0"
__date__ 	= "thu mar 28 2008"

import traceback
import exceptions
import pprint
import time

import MiniKSerial
import CheckThread

class TestException(exceptions.Exception):
	def __init__(self, msg):
		self.msg = msg
		return

	def __str__(self):
		print "","Test error: " + self.msg

def 	check_result(result_file, result_2_check):
	"""
	This function simply check if parsed result and
	result file are the same
	"""

	tmp = result_file.split('/')
	result = OpenFile(result_file + tmp[-2]  +  ".res")

	if result == (result_2_check.replace('-->>LEAK<<--\n','')):
		return 1

def	OpenFile(file_path, p = 1):
	"""
	Openfile and return its content
	"""
	try:
		testfile = open(file_path, "rb")
	except IOError:
		if p == 1:
			sys.stderr.write("The file " + file_path + " doesn't exist");
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
		test_list.insert(i + 1,["function", "check_" +  tmp_list[-3] + '_' + tmp_list[-2], tmp_list[-3]])
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

def	usage():
	print "./check.py WikiName K[1-5]"
	sys.exit(1)

def	main():
	i = 12			#on peut donner l'index de demarrage pour qu il se relance ou sa couille
				#il faut donc avant l afficher en cas de deco ou le sauvegarder
				#ne pas oublier de reload les files du test ds ce cas
				# if i != 0
				# loadfile de la section courrante
				# donc peut etre plus d indication
				# i num de seq de test et c num de test c[-1] == load file
				# if i != 0 && c[1].strip(' ') == "loadfile  " ...

	if ((sys.argv[1] == "") or (sys.argv[2] == "")):
		usage()

	test_list = ListTest()
	trace = open(sys.argv[1] + "-" + sys.argv[2] + ".inc.php", "w");
	trace.write("<?php\n");
	serial = MiniKSerial.MiniKSerial()

	while i < len(test_list):
		if test_list[i][0] == "loadfile":
			print "file loading ..."
			load_file(test_list[i][1])
			i += 1
		elif test_list[i][0] == "function":
			path = test_list[i - 1]
			testname = test_list[i][1]
			partname = test_list[i][2]

			arr = path.split('/');
			if arr[len(arr) - 2] == 'common':
				i += 1
				continue;

			try:
				checker = CheckThread.CheckThread(serial, trace)
				checker.setup(path, partname, testname)
				checker.setDaemon(True)
				checker.start()
				checker.join(30) # 30s timeout per test

			except KeyboardInterrupt:
				sys.stderr.write("\n\n=> user interrupt, leaving\n")
				sys.exit(1)

			if (checker.isAlive()):
				# timeout: kill old checker, get a new one
				sys.stderr.write("(timeout)\n")
				checker.trace_result_log(partname, testname, "0", "Timeout (30s)")
				checker.terminate()
				serial.die()
				serial = MiniKSerial.MiniKSerial()

			if (checker.new_serial_required):
				sys.stderr.write("Serial driver got renewed\n")
				serial = MiniKSerial.MiniKSerial()


		i += 1
		trace.flush()
	trace.write("?>\n")
	trace.close()


if __name__ == "__main__":
	from struct  import *
	from re      import *
	from pprint  import pprint
	import os, array, sys
	main()

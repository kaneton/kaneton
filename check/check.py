#!/usr/bin/python
"""
check.py 
This program is for testing kaneton
"""


__author__	= "jacob_s@lse.epitech.net"
__version__ 	= "0.0.1"
__date__ 	= ""


def 	check_result(result_file, result_2_check):
	print "result:"
	
	result = OpenFile(result_file)
	if result == result_2_check:
		return 1
	

def 	SendCommand(parsed_cmd):
	"""
	Send Command to execute on kaneton and receive the result, for sending
	it to the result checker
	"""
	
	result_buff = [] 
	result	= [] 
	
	data = ("command")
	tosend = (data)
	tosend_size = 8
	serial_send(tosend, tosend_size) #j'envoie que c des command qui vont suivre
	
	
	tosend = (parsed_cmd)
	tosend_size = (len(parsed_cmd[c]) + 1)
	serial_send(tosend, tosend_size)
			
	tmp = serial_recv()
	print tmp
	while tmp[1] == "printf":
		result += serial_recv()
		tmp = serial_recv()
			#result = serial_recv()
		print result
		result_buff += [result]
	i += 1
	
	return result_buff

def	OpenFile(file_path):
	"""
	Openfile and return its content
	"""
	try:
		testfile = open(file_path, "rb")
	except IOError:
		print "The file " + file_path + " doesn't exist"
		return
	file_content = testfile.read()
	testfile.close()
	return file_content
	
def	get_function_name(path, dir):
	return os.path.basename(path.rstrip("/") + "_" + dir.rstrip("/"))

def	get_res_file_name(path, dir):
	return path + dir + dir.rstrip("/") + ".res"
	
def	ListTest():
	"""
	ListTest : get Dir/testlist and scan testlist for all test 
	checksubdir and list sub-test
	"""

	dir_list = OpenFile("list").split()
	i = 0
	test_list = []
	while i < len(dir_list):
		file_path = OpenFile(dir_list[i] + "list").split()
		c = 0

		while c < len(file_path):
			current_path = dir_list[i] + file_path[c]
			test_list += [("new_test " + get_function_name(dir_list[i], file_path[c])).split()]	
			test_list += [get_function_name(dir_list[i], file_path[c])]
			test_list += [get_res_file_name(dir_list[i], file_path[c])]
			c += 1
		i += 1
	print "test_list: " 
	print test_list

	return test_list


if __name__ == "__main__":
	from kserial import *
	from struct  import *
	from re      import *
	import os, array

	serial_init("/dev/ttyS0")
	test_list = ListTest()
	total_ok = 0
	total_failed = 0
	i = 0

	while i < len(test_list):
		if test_list[i][0] == "new_test":
			print test_list[i][1]
		else:
			if len(test_list[i]) > 0:
				print "send_command:" + test_list[i]
				result = SendCommand(test_list[i])
#				parsed_result = parse_result(result)
				#pb viens de comment trouver la bonne fonction parse_result
				i += 1 
				if check_result(test_list[i], result):
					print "[OK]"
					total_ok += 1
				else:
					print "[FAILED]"
					total_failed += 1

		i += 1
	
	print "Passed: " + str(total_ok)
	print "Failed: " + str(total_failed)


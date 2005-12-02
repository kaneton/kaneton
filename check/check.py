#!/usr/bin/python
"""
check.py 
This program is for testing kaneton
"""


__author__	= "jacob_s@lse.epitech.net"
__version__ 	= "0.0.1"
__date__ 	= ""


#def	parse_result(result):
#	print "parsing_result:"
#	print result
#	parsed_result = result
#	return parsed_result[1]

def 	check_result(result_file, result_2_check):
	print "result:"
	
	result = OpenFile(result_file)
	print "real_result: "  + result
	print "test_result: " + result_2_check
	if result == result_2_check:
		return 1

def	get_function_addr(cmd):
	print "seek for function: " + cmd + " addr"
	
 	obj = os.popen("nm ../core/kaneton/kaneton | grep " + cmd +  " | cut -d ' ' -f 1")
	addr = obj.read()
	addr = addr.strip("\n")
	print "addr found at: "  + addr
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
	serial_send(tosend, tosend_size) #j'envoie que c des command qui vont suivre
	

	cmd = get_function_addr(cmd)	
	tosend = (cmd)
	tosend_size = (len(cmd) + 1)
	serial_send(tosend, tosend_size)
			
	tmp = serial_recv()
	result = [0,""]
	
	print tmp
	while tmp[1] == "printf":
		tmp = serial_recv()
		result[0] += tmp[0]
		result[1] += tmp[1]
		tmp = serial_recv()
		print result
	
	return result

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
			test_list += [dir_list[i]]
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
	import os, array, sys

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
				print "Launching test: " + test_list[i]
				result = SendCommand(test_list[i])
				i += 1
				sys.path.append(test_list[i])
				from parse_res import *
				parsed_result = parse_result(result)
				#pb viens de comment trouver la bonne fonction parse_result
				i += 1 
				if check_result(test_list[i], parsed_result):
					print "[OK]"
					total_ok += 1
				else:
					print "[FAILED]"
					total_failed += 1

		i += 1
	
	print "Passed: " + str(total_ok)
	print "Failed: " + str(total_failed)


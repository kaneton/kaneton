#!/usr/bin/python
""" moulin.py  serial test 
This program is for testing kaneton serial driver
"""


__author__	= "jacob_s@lse.epitech.net"
__version__ 	= "0.0.1"
__date__ 	= ""


def 	check_result(parse_cmd, result):
	print "result:"
	print  parse_cmd
	print result
	if parse_cmd == result:
		return 1
	

def 	SendCommand(parsed_cmd):
	"""
	Send Command to execute on kaneton and receive the result, for sending
	it to the result checker
	"""
	
	i = 0
	result_buff = [] 
	result	= [] 
	while i < len(parsed_cmd):
		current_cmd = parsed_cmd[i].split()
		if len(current_cmd) != 0:
			data = ("command")
			tosend = (data)
			tosend_size = 8
			serial_send(tosend, tosend_size) #j'envoie que c des command qui vont suivre
			tosend = (str(len(current_cmd)))
			tosend_size = (len(current_cmd) + 1)
			serial_send(tosend, tosend_size) # j'envoie le nnb d arg de la command  
			c = 0 
			while c < len(current_cmd):		# pour chaque arg de la cmd j envoie larg
				tosend = (current_cmd[c])
				tosend_size = (len(current_cmd[c]) + 1)
				serial_send(tosend, tosend_size)
				c += 1
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
		print "The file" + test_file + "doesn't exist"
		return
	file_content = testfile.read()
	testfile.close()
	return file_content
	
	
def	ListTest(test_path):
	"""
	ListTest : get Dir/testlist and scan testlist for all test 
	checksubdir and list sub-test
	"""

	dir_list = OpenFile(test_path + "testlist").split()
#	reg = search('^[^#]*', test_list) #virer les ocmmentaires
#	if reg:	test_list =  reg.group()
	i = 0
	test_list = []
	while i < len(dir_list):
		test_file_path = test_path + dir_list[i] + "testlist"
		file_path = OpenFile(test_path + dir_list[i] + "testlist").split()
		c = 0
		while c < len(file_path):
			test_list += [("new_test " + dir_list[i] + file_path[c]).split()]
			test_list += [OpenFile(test_path + dir_list[i] + file_path[c]).split("\n")]
			test_list += [OpenFile(test_path + dir_list[i] + file_path[c] + ".res").split()]
			c += 1
		i += 1
	
	return test_list


if __name__ == "__main__":
	from kserial import *
	from struct  import *
	from re      import *
	import os, array

	serial_init("/dev/ttyS0")
	test_list = ListTest("Test/")
	total_ok = 0
	total_failed = 0
	i = 0

#	print test_list
	while i < len(test_list):
		if test_list[i][0] == "new_test":
			print test_list[i][1]
		else:
			if len(test_list[i]) > 0:
				result = SendCommand(test_list[i])
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


#!/usr/bin/python
"""
check.py
This program is for testing kaneton
"""

__author__	= "jacob_s@lse.epitech.net, bucchi_m@lse.epita.fr"
__version__ 	= "0.1.0"
__date__ 	= "sun jun 18 2006"

html_nl="<br>"

def	html_head(stage, group):
	r = "<html><head><title>Trace moulette kaneton</title></head><body bgcolor=#C0C0C0><font face=verdana><center><h2>kaneton check trace</h2></center><br>"
	r = r + "<script langage='javascript'>function collapse(what) \
{ \
 var collapseRow = document.getElementById(what); \
 \
 if (collapseRow.style.display == '') \
 { \
  collapseRow.style.display = 'none'; \
 } \
 else \
 { \
  collapseRow.style.display = ''; \
 } \
}</script>"
	r = r + "<table width='33%'>"
	r = r + "<tr><td width='50%'>Stage:</td><td>" + stage + "</td></tr>"
	r = r + "<tr><td width='50%'>Group:</td><td>" + group + "</td></tr>"
	r = r + "</table>"
	r = r + "<hr><table><tr><td width=100px><h3>Test suite:</h3></td>"
	r = r + "<td bgcolor='#46a31b' width=30px>OK</td><td bgcolor='#d18d03' width=30px>Failed</td><td bgcolor='#d8ff00' width=30px>OK but Memleaks</td><td bgcolor='#d800ff' width=30px>Timeout</td></tr></table><br><br><center>"
	return r

def	html_foot():
	r = "<p align='right'><a href='http://www.kaneton.org'>www.kaneton.org</a></p>"
	r = r + "<br></font></body></html>"
	return r

def	html_summary(group):
	return "</center><hr><h3>Summary for " + group + ":</h3>"

def	html_test_head(id ,test, col):
	r = "<table width='90%' border cellspacing=0>"
	r = r + "<tr bgcolor='" + col + "'><td><a href='javascript:collapse(\"id_" + str(id) + "\");' style='text-decoration:none'>+</a> " + test + " </td></tr><tr><td>"
	r = r + "<dd id='id_" + str(id) + "' style='display:none'>"
	return r

def	html_test_foot():
	return "</dd></td></tr></table><br>"


def 	check_result(result_file, result_2_check):
	"""
	This function simply check if parsed result and
	result file are the same
	"""

	tmp = result_file.split('/')
	result = OpenFile(result_file + tmp[-2]  +  ".res")

	if result == (result_2_check.replace('-->>LEAK<<--\n','')):
		return 1

def	get_function_addr(cmd):
 	"""
	Return addr of function named cmd in kaneton
	"""

	obj = os.popen("nm ../kaneton/core/core | grep " + cmd.replace('-', '_') +  " | cut -d ' ' -f 1")
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

	cmd = cmd + "/" + get_function_addr(cmd)
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


def	main():
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
	total_leak = 0

	serial_init(sys.argv[1])
	test_list = ListTest()

	print html_head(sys.argv[2], sys.argv[3])
	while i < len(test_list):
		if test_list[i][0] == "loadfile":
			print "file loading ..."
			load_file(test_list[i][1])
			i += 1
		elif test_list[i][0] == "function":
			try:
				sys.stderr.write("test: " + test_list[i - 1] + "\n")
				serial_timeout(300000);
				result = SendCommand(test_list[i][1])
				parsed_result = set_and_parse_result(test_list[i - 1], result)
				if check_result(test_list[i - 1], parsed_result):
					if parsed_result.find('-->>LEAK<<--') != -1:
						col = '#d8ff00'
						total_leak += 1
					else:
						col = '#46a31b'
					total_ok += 1
				else:
					col = '#d18d03'
					total_failed += 1
				print html_test_head(i, test_list[i - 1], col)
				print parsed_result.replace('\n', html_nl)
				print html_test_foot()
			except Exception, e:
				print html_test_head(i, test_list[i - 1], '#d800ff')
				print "Timeout or fatar error"
				print html_test_foot()
				total_failed += 1
				sys.stderr.write("/!\\ PLEASE REBOOT TEST MACHINE /!\\\n")
				serial_close()
				retr = sys.stdin.readline().replace('\n','')
				if retr == "":
					serial_init(sys.argv[1])
				else:
					sys.stderr.write("retrying on different port " + retr + "\n")
					serial_init(retr)
			i += 1
		else:
			i += 1

	print html_summary(sys.argv[3])
	print "Passed: " + str(total_ok) + html_nl
	print "Failed: " + str(total_failed) + html_nl
	print "Leaks: " + str(total_leak)
	print html_foot()

if __name__ == "__main__":
	from kserial import *
	from struct  import *
	from re      import *
	import os, array, sys
	try:
		main()
	except Exception, e:
		print "<font color='#FF0000'><h1>Aborted</h1></font>"
		print html_foot()

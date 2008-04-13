
'''
* SUMMARY

Thread encapsulation of the kaneton check-over-serial protocol

The thread object must be initialized with a kaneton-serial instance and
a file which will be used as a trace.

The thread can be setup multiples times (one for each test) without resetting
the trace or the serial driver.

This allows precise and reliable timeouts for the whole test operation.

* REQUIREMENTS

- kaneton_reboot must be in your path, and must reboot the checker
- ../kaneton/kaneton must be the currently tested kaneton binary
- python ctypes library if python version <= 2.4

* USAGE

Instanciate using CheckThread.CheckThread(xxx)

Beware: the given serial line will be closed on object destruction!

* AUTHORS

Elie BLETON (elie@lse.epita.fr)

Legacy:
Matthieu BUCCHIANERI (bucchi_m@lse.epita.fr)
S******* JACOB? (jacob_s@lse.epita.fr)

'''

import os
import sys
import threading
import exceptions

import ctypes
import inspect

import MiniKSerial


#
# UTILITY ROUTINES
#

def check_result(result_file, result_2_check):
    """
    This function simply check if parsed result and
    result file are the same
    """

    tmp = result_file.split('/')
    result = read_file(result_file + tmp[-2]  +  ".res")
    if result == (result_2_check.replace('-->>LEAK<<--\n','')):
        return True
    return False

def read_file(file_path, p = 1):
    """
    Opens a file and return its content
    """
    try:
        testfile = open(file_path, "rb")
    except IOError:
        sys.stderr.write("The file " + file_path + " doesn't exist");
        return ""
    file_content = testfile.read()
    testfile.close()
    return file_content


def get_function_addr(cmd):
    """Return addr of function named cmd in kaneton"""

    obj = os.popen("nm ../kaneton/kaneton | grep " + cmd.replace('-', '_') +  " | cut -d ' ' -f 1")
    addr = obj.read()
    addr = addr.strip("\n")
    return addr

#
# CHECK EXCEPTION
#

class CheckException(exceptions.Exception):
	def __init__(self, msg):
		self.msg = msg
		return

	def __str__(self):
		print "","Check error: " + self.msg

#
# PYTHON THREAD MAGIC
# see http://
#

def _async_raise(tid, exctype):
    """raises the exception, performs cleanup if needed"""
    if not inspect.isclass(exctype):
        raise TypeError("Only types can be raised (not instances)")
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, ctypes.py_object(exctype))
    if res == 0:
        raise ValueError("invalid thread id")
    elif res != 1:
        # """if it returns a number greater than one, you're in trouble,
        # and you should call it again with exc=NULL to revert the effect"""
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, 0)
        raise SystemError("PyThreadState_SetAsyncExc failed")

#
# CHECK THREAD
#

class CheckThread(threading.Thread):
    def __init__(self, serial, trace):
        if (not isinstance(serial, MiniKSerial.MiniKSerial)):
            raise TypeError("You should provide a valid kaneton-serial instance as 1st parameter")

        if (not (type(trace) is file)):
            raise TypeError("You should provide a valid file as 2nd parameter")

        threading.Thread.__init__(self)
        self.result = None
        self.__setup = False
        self.serial = serial
        self.trace_file = trace
        self.user_interrupt = False

    def setup(self, testpath, partname, testname):
        self.__setup = True
        self.testpath = testpath
        self.partname = partname
        self.testname = testname

    def run(self):
        if (self.__setup == False):
            raise NameError("Invoke setup(...) prior launching the thread")

        try:
            sys.stderr.write("test: ["+self.partname+"] "+self.testname+" ... ");
            self.check(self.testpath, self.partname, self.testname)
        except CheckException, e:
            self.trace_result_log(self.partname, self.testname, 0, "Fatal error ("+e.msg+")")
        except MiniKSerial.SerialException, e:
            sys.stderr.write("serial failure.\n");
            self.trace_result_log(self.partname, self.testname, 0, "Fatal serial error ("+e.msg+")")
        except KeyboardInterrupt:
            self.user_interrupt = True

    def check(self, testpath, partname, testname):
        testid = testpath.replace("/", "_").replace("-", "_").rstrip("_").lstrip("kaneton");
        os.system("kaneton_reboot");

        hello = self.serial.read_trame()
        if hello != 'Ready!':
            sys.stderr.write("(not ready) ");
            raise CheckException('Not ready on time');

	result = self.send_command(testname)
	if check_result(testpath, result):
            if result.find('-->>LEAK<<--') != -1:
		sys.stderr.write("passed (leak).\n");
                self.trace_result(partname, testname, "0.5")
            else:
                sys.stderr.write("passed.\n");
                self.trace_result(partname, testname, "1")
        else:
            sys.stderr.write("diff failed.\n");
            self.trace_result_log_and_output(partname, testname, "0", "Diff fails", result)


    def send_command(self, testname):
	"""Send Command to execute on kaneton and receive the result"""

	result = ""
	self.serial.write_trame("command")
	self.serial.write_trame(testname + "/" + get_function_addr(testname))
	trame = self.serial.read_trame()

	while trame != "endprintf":
            if trame == "printf":
                result = result + self.serial.read_trame()
                trame = self.serial.read_trame()
            elif trame == "Ready!":
                sys.stderr.write("reboot is failure.\n");
                raise CheckException('Unexpected reboot')
            else:
                sys.stderr.write("(unk. code) ");
                raise CheckError('Invalid command')

        return result

    def _get_my_tid(self):
        """determines this (self's) thread id"""
        if not self.isAlive():
            raise threading.ThreadError("the thread is not active")

        # do we have it cached?
        if hasattr(self, "_thread_id"):
            return self._thread_id

        # no, look for it in the _active dict
        for tid, tobj in threading._active.items():
            if tobj is self:
                self._thread_id = tid
                return tid

        raise AssertionError("could not determine the thread's id")


    def terminate(self):
        self.serial.die()
        _async_raise(self._get_my_tid(), SystemExit)


    #
    # TRACE RELATED ROUTINES
    #

    def trace(self, line):
        self.trace_file.write(line + "\n")

    def trace_result(self, partname, testname, result):
        self.trace("$trace['"+partname+"']['"+testname+"'] = "+str(result)+";")

    def trace_result_log(self, partname, testname, result, log):
        self.trace_result(partname, testname, result)
        self.trace("$log['"+partname+"']['"+testname+"'] = '"+log.replace("'", "\'")+"';")

    def trace_result_log_and_output(self, partname, testname, result, log, output):
        self.trace_result_log(partname, testname, result, log)
        self.trace("$output['"+partname+"']['"+testname+"'] = '"+output.replace("'", "\'")+"';")

from kserial import *
from struct  import *
from re      import *
from PIL     import *
import os, array, sys

funcs = [""];

def	get_func(name):
	i = 0;
	while i < len(funcs):
		if funcs[i] == name:
			return i;
		i += 1;
	funcs.insert(i, name);
	return i;

def     main():

	log = open("log", "w");
	serial_init(sys.argv[1]);
	serial_timeout(300000);
	t = 0;
        while 1:
		tmp = serial_recv();
		if tmp[1] == "eof":
			break;
		func = get_func(tmp[1]);
		cycle = serial_recv_cycle();
		if (cycle < 0):
			if (cycle == -1):
				log.write("- " + tmp[1] + " switch to RUNNING state\n");
			if (cycle == -2):
				log.write("- " + tmp[1] + " switch to STOPPED state\n");
			if (cycle == -3):
				log.write("- " + tmp[1] + " switch to BLOCKED state\n");
		else:
			log.write("+ " + tmp[1] + " executed for " + str(cycle) + "x10^3 cycles\n");
			sys.stdout.write(str(t) + "\t" + str(func) + "\n");
			t += cycle;
			sys.stdout.write(str(t) + "\t" + str(func) + "\n");
        log.close();
main()

#! /usr/bin/python
"""
check.py
"""

TST_PATH = "tests"
REF_PATH = "ref"
TRC_PATH = "traces"
TAR_PATH = "tarballs"
TMP_PATH = "tmp"
BIOS_PATH = "bios"

import env
import os, array, sys, time, glob, subprocess, popen2

__author__ = "lec_l@lse.epita.fr"
__version__ = "1.0b"
__date__ = "mon feb 20 2008"

OUT_PATH = TMP_PATH + "/output"
SRC_PATH = TMP_PATH + "/src"
obj = TMP_PATH + "/bootsector.o"
bin = TMP_PATH + "/bootsector"


#
# init()
#
# Initialize.
#
def	init() :

	devnull = open("/dev/null", 'w+')

	# Init dirs...
	p = subprocess.Popen(["rm", "-rf", TMP_PATH, TRC_PATH],
			     stdout=devnull, stderr=devnull)
	p.wait()
	os.mkdir(TMP_PATH)
	os.mkdir(TRC_PATH)
	os.mkdir(OUT_PATH)
	os.mkdir(SRC_PATH)

	# Compile elf
	p = subprocess.Popen(["make", "-C", "tests/elf_binary", "all"],
			     stdout=devnull, stderr=devnull)
	p.wait()

	devnull.close()
	return


#
# config()
#
# Display configuration
#
def	config() :

	env.display(env.HEADER_OK, "checking k0", env.OPTION_NONE)
	env.display(env.HEADER_NONE, "", env.OPTION_NONE)
	env.display(env.HEADER_OK, "current configuration:", env.OPTION_NONE)
	env.display(env.HEADER_OK, "  reference path:         " + REF_PATH,
		    env.OPTION_NONE)
	env.display(env.HEADER_OK, "  tarballs path:          " + TAR_PATH,
		    env.OPTION_NONE)
	env.display(env.HEADER_OK, "  tests path:             " + TST_PATH,
		    env.OPTION_NONE)
	env.display(env.HEADER_OK, "  traces path:            " + TRC_PATH,
		    env.OPTION_NONE)
	env.display(env.HEADER_OK, "  temporary folder:       " + TMP_PATH,
		    env.OPTION_NONE)
	env.display(env.HEADER_NONE, "", env.OPTION_NONE)


	if os.path.exists(TMP_PATH) :
		env.display(env.HEADER_ERROR,
			    "`" + TMP_PATH + "' will be deleted",
			    env.OPTION_NONE)
		env.display(env.HEADER_NONE, "", env.OPTION_NONE)

	env.display(env.HEADER_INTERACTIVE,
		    "to cancel press CTRL^C, otherwise press enter",
		    env.OPTION_NONE)
	env.input(env.OPTION_NONE)

	return


#
# clean()
#
# Clean temporary files.
#
def	clean() :

	devnull = open("/dev/null", 'w+')

	env.display(env.HEADER_OK, "cleaning temporary files", env.OPTION_NONE)
#	p = subprocess.Popen(["rm", "-rf", TMP_PATH])
#	p.wait()

	# Elf
	p = subprocess.Popen(["make", "-C", "tests/elf_binary", "clean"],
			     stdout=devnull, stderr=devnull)
	p.wait()

	devnull.close()

	return

#
# check_tarball(out_path, src_path)
#
# Desc.
#
# out_path : path to the folder where output should be stored
# src_path : path to the folder containing the exercices folders.
#
def	check_tarball(out_path, src_path) :

	# getting list of exercices in the test folder
 	exercices = glob.glob(TST_PATH + "/ex[0-9]*")
	exercices.sort()
	# for each exercice
	for i in range (0, len(exercices)) :
		# name of the exercice
		ex_name = os.path.basename(exercices[i])
		# create a folder for the exercice in the output folder
		os.mkdir(out_path + "/" + ex_name)

		# getting list of tests for the current exercice
		tests = glob.glob(TST_PATH + "/" + ex_name + "/*.S")
		tests.sort()
		# for each test
		for j in range (0, len(tests)) :
			tst_name = os.path.basename(tests[j]).replace(".S", "")
			tst_file = tests[j]
			out_file = out_path + "/" + ex_name + "/" + tst_name
                        logfile = open(src_path + "/" + ex_name + "_" + tst_name + ".log", 'w+')

			env.display(env.HEADER_OK,
				    "      " + ex_name + " - " + tst_name,
				    env.OPTION_NONE)

			# compile test file
                        args = "gcc -I " + TST_PATH + " -I " + src_path + \
			    " -c " + tst_file + " -o " + obj
                        p = subprocess.Popen(args.split(' '),
					     stdout=logfile, stderr=logfile)
			p.wait()
			if p.returncode != 0 :
				env.display(env.HEADER_ERROR,
					    "  compilation failed, abort!",
					    env.OPTION_NONE)
				continue

			# produce a flat binary with previous output object
			args = "ld " + obj + " -o " + bin + \
			    " --Ttext 0x7c00 --oformat binary"
			p = subprocess.Popen(args.split(' '),
					     stdout=logfile, stderr=logfile)
			p.wait()
			if p.returncode != 0 :
				env.display(env.HEADER_ERROR,
					    "  linkage failed, abort!",
					    env.OPTION_NONE)
				continue


			if ex_name == "ex6" :
				p = subprocess.Popen(["mv",
						      bin,
						      "bootsector_tmp"],
						     stdout=logfile,
						     stderr=logfile)
				p.wait()

				p = subprocess.Popen(["bash",
						      "-c",
						      "cat bootsector_tmp tests/elf_binary/LOADME.elf /dev/zero | dd bs=512 count=2880 of=tmp/bootsector"],
						     stdout=logfile,
						     stderr=logfile)
				p.wait()

				p = subprocess.Popen(["rm",
						      "-f",
						      "bootsector_tmp"],
						     stdout=logfile,
						     stderr=logfile)
				p.wait()


			# run qemu with the flat binary and
			# redirect serial port to output file

			args = "qemu-system-i386 -fda " + bin + " -nographic -serial file:" + out_file
			p = subprocess.Popen(args.split(' '),
					     stdout=logfile, stderr=logfile)

			time.sleep(1)
			os.kill(p.pid, 9)
                        logfile.close()

	return


#
# reference()
#
# Desc.
#
def	reference() :

	env.display(env.HEADER_OK, "generating reference output...",
		    env.OPTION_NONE)
	os.mkdir(OUT_PATH + "/" + REF_PATH)
	check_tarball(OUT_PATH + "/" + REF_PATH, REF_PATH)
	env.display(env.HEADER_NONE, "", env.OPTION_NONE)

	return


#
# students()
#
# Desc.
#
def	students() :

	devnull = open("/dev/null", 'w+')
	env.display(env.HEADER_OK, "students:", env.OPTION_NONE)

	# getting list of tarballs in the student folder
 	tarballs = glob.glob(TAR_PATH + "/" + "*.tar.bz2")
	tarballs.sort()
	# for each tarball
	for i in range(0, len(tarballs)) :
                src = tarballs[i].replace("tarballs/", "").replace(".tar.bz2", "")
                login = os.path.basename(src).replace("-k0", "")
		# the tarball is untared in the tarball directory
                os.mkdir(SRC_PATH + "/" + src)
		args = "tar -xjf " + tarballs[i] + " -C " + SRC_PATH + "/" + src
		p = subprocess.Popen(args.split(' '),
				     stdout=devnull, stderr=devnull)
		p.wait()
		if p.returncode != 0 :
			env.display(env.HEADER_ERROR,
				    "    couldn't untar " + tarballs[i] + " !",
				    env.OPTION_NONE)
			continue

		env.display(env.HEADER_OK, "  " + login + "...",
			    env.OPTION_NONE)

		env.display(env.HEADER_OK, "    Running tests...",
			    env.OPTION_NONE)

		os.mkdir(OUT_PATH + "/" + src)
		check_tarball(OUT_PATH + "/" + src, SRC_PATH + "/" + src)

		env.display(env.HEADER_OK, "    Comparing outputs...",
			    env.OPTION_NONE)

		# get trace name
		traceno = 1
		tracepath = TRC_PATH + "/" + login + '-k0-' + str(traceno) + '.csv'
		while os.path.exists(tracepath):
			traceno = traceno + 1
			tracepath = TRC_PATH + "/" + login + '-k0-' + str(traceno) + '.csv'

		# open trace
		trace = open(tracepath, 'w+')
		out_ref = OUT_PATH + "/" + REF_PATH
		stu_ref = OUT_PATH + "/" + src

		# getting list of exercices in the test folder
		exercices = os.listdir(out_ref)
		exercices.sort()

                tpassed = 0
                ttotal = 0
                details = ""
		# for each exercice
		for i in range (0, len(exercices)) :

			# name of the exercice
			ex_name = os.path.basename(exercices[i])

			# getting list of output files for the current exercice
			out = os.listdir(out_ref + "/" + ex_name)
			out.sort()
			# for each test
			for j in range (0, len(out)) :
                                ttotal = ttotal + 1
                                ref = out_ref + "/" + ex_name + "/" + out[j]
				stu = stu_ref + "/" + ex_name + "/" + out[j]
				args = "diff -u " + ref + " " + stu
				p = subprocess.Popen(args.split(' '),
						     stdout=devnull,
						     stderr=devnull)
				p.wait()
				if (p.returncode == 0):
                                        tpassed = tpassed + 1
                                        details += ",1"
                                else:
                                        details += ",0"

                tresult = float(tpassed) / ttotal
                env.display(env.HEADER_OK,
                            "      " + str(tpassed) + "/" + str(ttotal),
                            env.OPTION_NONE)

		trace.write(login + "," + str(tresult) + details + "\n")
		trace.close()
		env.display(env.HEADER_NONE, "", env.OPTION_NONE)

	env.display(env.HEADER_NONE, "", env.OPTION_NONE)

	devnull.close()
	return


#
# main()
#
# Entry point of the script.
#
def     main() :

	config()
	init()
	reference()
	students()
#	clean()

        return 0


if __name__ == "__main__" :
	main()

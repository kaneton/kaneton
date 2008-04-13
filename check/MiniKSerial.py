#!/usr/bin/python

'''
* SUMMARY

Tiny implementation of the kaneton serial protocol

This doesn't require the kaneton kserial module, it uses pyserial
instead. Install it from http://pyserial.sourceforge.net

This driver doesn't use timeouts, so beware !
Timeouts are bad for serial read/write as they can interrupt a live
transmission.

* USAGE

Instanciate using MiniKSerial.MiniKSerial(serial_line)
Default value for serial_line should be fine for kaneton usage
Otherwise consult the pyserial documentation for available options

Beware: the given serial line will be closed on object destruction!

* AUTHORS

Elie BLETON (elie@lse.epita.fr)

'''

import serial
import struct
import exceptions
import sys
import time

class SerialException(exceptions.Exception):
	def __init__(self, msg):
		self.msg = msg
		self.dead = False
		return

	def __str__(self):
		print "","Test error: " + self.msg

class MiniKSerial:
    def __init__(self, serial_line = False):
        self.serial_magic = 0xF4859632

        if (serial_line != False):
            assert(type(serial_line) is serial.Serial)
            self.serial_line = serial_line
        else:
            # defaults to first serial port at 57600 bauds (kaneton)
            self.serial_line = serial.Serial('/dev/ttyS0', 57600)

    def __del__(self):
	    if (not self.dead):
		    self.die()

    def die(self):
	    self.serial.flushInput()
	    self.serial.flushOutput()
	    self.serial_line.close()
	    self.dead = True

    def checksum(self, data):
        '''This checksum is the worst I've ever seen.
        I'm just following the protocol here, don't blame me.'''
        csum = 0
        for c in data:
            csum = csum ^ ord(c)
        return csum

    def read_trame(self):
        size = struct.unpack("<I", self.serial_line.read(4))
        magic = struct.unpack("<I", self.serial_line.read(4))
        crc = struct.unpack("<I", self.serial_line.read(4))
        self.serial_line.read(4) # junk data as per protocol

        if (magic[0] != self.serial_magic):
            raise SerialException("Bad magic while reading")

        buf = self.serial_line.read(size[0])

        if (crc[0] != self.checksum(buf)):
            self.serial_line.write("badcrc")
            raise SerialException("Bad CRC while reading")
        else:
            self.serial_line.write("crc-ok")

        return buf

    def write_trame(self, data):
        format = "<IIII" + str(len(data) + 1) + "s"
        trame = struct.pack(format,
                            len(data) + 1,
                            self.serial_magic,
                            self.checksum(data),
                            0, data)
        self.serial_line.write(trame)

        ack = self.serial_line.read(6)
        if (ack == "badcrc"):
            raise SerialException("Bad CRC while sending")

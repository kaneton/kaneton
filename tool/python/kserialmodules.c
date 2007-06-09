/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/tools/python/kserialmodules.c
 *
 * created       solal jacob   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [sun jun 18 17:50:56 2006]
 */

#include <Python.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/*
 * serial port speed.
 */

#define IOSPEED		B57600

/*
 * types used for data transfer.
 */

typedef struct		s_data
{
  unsigned int		size;
  unsigned int		magic;
  unsigned int		crc;
  unsigned char*	data;
}			t_data;

typedef int (*t_fun_op)(int, void*, size_t);

/*
 * the file descriptor of our opened serial port.
 */

int			fd = -1;

/*
 * the options of our serial port.
 */

struct termios		options;


/*
 * this function is used to read/write with multiple retry.
 */

int			readwrite(t_fun_op			op,
				  int				fd,
				  char*				buf,
				  size_t			len)
{
  register int		r;

  do
    r = op(fd, buf, len);
  while ((r == -1 && (errno == EINTR)));

  return r;
}

/*
 * this function is used to  read/write data of large size in multiple
 * packets.
 */

size_t			allreadwrite (t_fun_op			op,
				      int			fd,
				      char*			buf,
				      size_t			len)
{
  size_t		written = 0;
  size_t		total = len;
  char*			buff = buf;

  while (len)
    {
      int w = readwrite(op, fd, buf, len);
      if (!w)
	{
	  errno = EPIPE;
	}
      if (w <= 0)
	break;
      written += w;
      buf += w;
      len -= w;
    }
/*
  fprintf(stderr, "%s %d bytes out of %d\n",
	  op == read ? "read" : "wrote", written, total);
  fprintf(stderr, "--%s--\n", buff);
*/
  return written;
}

/*
 * compute checksum of a data chunk.
 */

unsigned int		chk_sum(void*				data,
				size_t				size)
{
  unsigned int		crc = 0;
  unsigned char 	*cdata;

  cdata = data;
  while(size--)
    crc ^= *cdata++;

  return crc;
}

/*
 * this is a python exported function. this function receive data from
 * the opened serial port.
 *
 * steps:
 *
 * 1) receive the header packet.
 * 2) receive the data itself.
 * 3) send ack or error.
 */

static PyObject*	kserial_serial_recv(PyObject*		self,
					    PyObject*		args)
{
  t_data		rdata;
  int			n = 0;
  PyObject*		ret;

  /*
   * 1)
   */

  n = allreadwrite(read, fd, (void*)&rdata, sizeof(rdata));
  if (n == -1)
    {
      fprintf(stderr, "serial_recv: Couldn't get packet header\n");
      return Py_BuildValue("i", -1);
    }
  if (n == 0 || errno == EPIPE)
    {
      fprintf(stderr, "serial_recv: Connection time-out\n");
      return Py_BuildValue("i", -1);
    }
  if (rdata.magic != 0xF4859632)
    {
      fprintf(stderr, "serial_recv: Bad magic\n");
      return Py_BuildValue("i", -1);
    }
  rdata.data = malloc((rdata.size + 1) * sizeof(unsigned char));

  /*
   * 2)
   */

  n = allreadwrite(read, fd, (void*)rdata.data, rdata.size);
  *(rdata.data + rdata.size) = 0;

  if (n != rdata.size || errno == EPIPE)
    {
      fprintf(stderr, "serial_recv: Connection time-out\n");
      return Py_BuildValue("i", -1);
    }

  /*
   * 3)
   */

  if (n == -1)
    {
      free(rdata.data);
      fprintf(stderr, "serial_recv: Couldn't get packet\n");
      return Py_BuildValue("i", -1);
    }
  if (rdata.crc == chk_sum(rdata.data, rdata.size))
    {
      allreadwrite(write, fd, "crc-ok" , 6);
      return Py_BuildValue("is", rdata.size, rdata.data);
    }
  else
    {
      fprintf(stderr, "serial_recv: Bad crc\n");
      allreadwrite(write, fd, "badcrc", 6);
      free(rdata.data);
      return Py_BuildValue("i", -1);
    }
}

/*
 * this  function is python-exported.  it sends  data over  the serial
 * port.
 *
 * steps:
 *
 * 1) send the packet header with size and checksum.
 * 2) send the packet itself.
 * 3) read the acknoledgment.
 */

static PyObject*	kserial_serial_send(PyObject*		self,
					    PyObject*		args)
{
  int			size, i, n;
  char			status[7];
  char*			data;
  t_data 		sdata;

  if (!PyArg_ParseTuple(args, "si",  &data, &size))
    {
      fprintf(stderr, "serial_send: Can't get arg\n");
      exit(0);
    }

  /*
   * 1)
   */

  sdata.crc = chk_sum(data, size);
  sdata.size = size;
  sdata.magic = 0xF4859632;

  if (allreadwrite(write, fd, (void*)&sdata, sizeof(sdata)) == -1)
    {
      fprintf(stderr, "serial_send: Couldn't send packet header\n");
      return Py_BuildValue("i", -1);
    }

  /*
   * 2)
   */

  if (allreadwrite(write, fd, data, size) == -1)
    {
      fprintf(stderr, "serial_send: Couldn't send packet\n");
      return Py_BuildValue("i", -1);
    }

  /*
   * 3)
   */

  if ((n = allreadwrite(read, fd, status, 6)) == -1)
    {
      fprintf(stderr, "serial_send: Couldn't read acknowledgment\n");
      return Py_BuildValue("i", -1);
    }
  status[6] = '\0';
  if (!strcmp(status, "crc-ok"))
    return Py_BuildValue("i", 0);
  else
    {
      fprintf(stderr, "serial_send: Bad crc\n");
      return Py_BuildValue("i", -1);
    }
}

/*
 * this function closes the serial port. python-exported.
 */

static PyObject*	kserial_serial_close(PyObject*		self,
					     PyObject*		args)
{
  close(fd);

  return Py_BuildValue("i", 0);
}

/*
 * this function opens the specified serial device. python-exported.
 *
 * steps:
 *
 * 1) open the device.
 * 2) set the options.
 */

static PyObject*	kserial_serial_init(PyObject*		self,
					    PyObject*		args)
{
  const char*		device;

  if (!PyArg_ParseTuple(args, "s", &device))
    {
      fprintf(stderr, "serial_init: Can't get device arg\n");
      return Py_BuildValue("i", -1);
    }

  /*
   * 1)
   */

  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
    {
      fprintf(stderr, "serial_init: Couldn't open the port %s\n", device);
      return Py_BuildValue("i", -1);
    }
  else
    fcntl(fd, F_SETFL, 0);

  /*
   * 2)
   */

  if (tcgetattr(fd, &options) == -1)
    {
      fprintf(stderr, "serial_init: Couldn't getattr\n");
      return Py_BuildValue("i", -1);
    }

  cfsetispeed(&options, IOSPEED);
  cfsetospeed(&options, IOSPEED);
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag |= CRTSCTS;
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag &= ~OPOST;
  options.c_iflag = IGNPAR | IXANY;
  options.c_cc[VTIME] = 100;
  options.c_cc[VMIN] = 0;

  if (tcsetattr(fd, TCSANOW, &options) == -1)
    {
      fprintf(stderr, "serial_init: Couldn't setattr\n");
      return Py_BuildValue("i", -1);
    }

  return Py_BuildValue("i", 0);
}

/*
 * this  python exported  function  is  used to  set  the serial  port
 * timeout.
 */

static PyObject*	kserial_serial_timeout(PyObject*	self,
					       PyObject*	args)
{
  int			timeout;

  if (!PyArg_ParseTuple(args, "i", &timeout))
    {
      fprintf(stderr, "serial_timeout: Can't get timeout arg\n");
      return Py_BuildValue("i", -1);
    }

  options.c_cc[VTIME] = timeout / 100;

  if (tcsetattr(fd, TCSANOW, &options) == -1)
    {
      fprintf(stderr, "serial_init: Couldn't setattr\n");
      return Py_BuildValue("i", -1);
    }

  return Py_BuildValue("i", 0);
}

/*
 * this is the export table.
 */

static PyMethodDef kserialMethods[] =
  {
    { "serial_init",
      (PyCFunction)kserial_serial_init,
      METH_VARARGS,
      "init serial connection" },
    { "serial_recv",
      (PyCFunction)kserial_serial_recv,
      METH_VARARGS,
      "serial receive"},
    { "serial_send",
      (PyCFunction)kserial_serial_send,
      METH_VARARGS,
      "serial send" },
    { "serial_close",
      (PyCFunction)kserial_serial_close,
      METH_VARARGS,
      "serial close" },
    { "serial_timeout",
      (PyCFunction)kserial_serial_timeout,
      METH_VARARGS,
      "set serial timeout" },
    { NULL, NULL, 0, NULL}
  };

PyMODINIT_FUNC initkserial(void)
{
  (void) Py_InitModule("kserial", kserialMethods);
}

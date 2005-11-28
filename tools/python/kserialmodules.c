#include	<Python.h>

#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<string.h>
#include	<stdlib.h>
#include 	<errno.h>


int			fd;
struct termios		options;
	
struct			s_data		
{
	unsigned int 	size;
	unsigned int 	magic;
	unsigned int 	crc;
	unsigned char	*data;
};
typedef	struct s_data	t_data;



int	readwrite(int (*op)(int, char *, int) ,int fd, char const *buf, unsigned int len)
{
  register int r;
  do r = (*op)(fd, (char *)buf, len);
  while ((r == -1 && (errno == EINTR)));
  return r ;
}

unsigned int allreadwrite (int (*op) (int, char *, int), int fd, register char *buf, register unsigned int len)
{
  register unsigned int written = 0 ;
  while (len)
  {
    register int w = readwrite((*op), fd, buf, len) ;
    if (!w) errno = EPIPE ;
    if (w <= 0) break ;
    written += w ;
    buf += w ;
    len -= w ;
  }
  return written ;
}


unsigned int		chk_sum(void *data, unsigned int size)
{
	unsigned int	crc = 0;
	unsigned char 	*cdata;
	
	cdata = data;
	while(size--)
       	  crc ^= *cdata++;

	return (crc);
}

static	PyObject	*kserial_serial_recv(PyObject *self, PyObject *args)
{
	t_data 	rdata;
	int	n = 0;
	PyObject *ret;
	
	n = allreadwrite(read, fd, (char *) &rdata, sizeof(rdata));
	if (n == -1 || rdata.magic != 0xF4859632)
	   return Py_BuildValue("i", -1);
	rdata.data = malloc((rdata.size + 1) * sizeof(unsigned char));
	
	n = allreadwrite(read, fd, rdata.data, rdata.size);
	*(rdata.data + rdata.size)  = 0;  /* met un 0 pour les chaines */
	if (n == -1) 
	{
	  free(rdata.data);
	  return Py_BuildValue("i", -1);
	}
	if (rdata.crc == chk_sum(rdata.data, rdata.size))
	  {
	    allreadwrite(write, fd, "crc-ok" , 6); 
	    return Py_BuildValue("is", rdata.size, rdata.data); 
	  }
	else
	 {
	  allreadwrite(write, fd, "badcrc", 6);
	  free(rdata.data);
	  return Py_BuildValue("i", -1);
	 }
}


static	PyObject	*kserial_serial_send(PyObject *self, PyObject *args)
{
	int		size, i, n;
	char 		status[7];
	char		*data;
	t_data 		sdata;

	if (!PyArg_ParseTuple(args, "si",  &data, &size)) 
	{
		printf("serial_send Can't get arg\n");
		exit(0);
	}

	sdata.crc = chk_sum(data, size);
	sdata.size = size;
  	sdata.magic = 0xF4859632;
		
	n = allreadwrite(write, fd,(char *) &sdata, sizeof(sdata));
	n = allreadwrite(write, fd, data, size);
	if ((n = allreadwrite(read, fd, status, 6)) == -1)
		return Py_BuildValue("i", -1);
	status[6] = '\0';
	if (!strcmp(status, "crc-ok"))
	   return Py_BuildValue("i", 0);
	else
	   return Py_BuildValue("i", -1);
}


static 	PyObject	*kserial_serial_init(PyObject *self, PyObject *args)
{
	const char	*device;


	if (!PyArg_ParseTuple(args, "s", &device))
	{ 
	  printf("Can't get device arg\n");
	  return Py_BuildValue("i", -1);	
	}
		
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{	
	   printf("Couldn't open the port %s\n", device);
	   return Py_BuildValue("i", -1);
	}
	else
	  fcntl(fd, F_SETFL, 0);
	
	tcgetattr(fd, &options);
	
	cfsetispeed(&options, B57600);
	cfsetospeed(&options, B57600);
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
	tcsetattr(fd, TCSANOW, &options);

	return Py_BuildValue("i", -1);
}

static PyMethodDef kserialMethods[] =
{
	{ "serial_init", (PyCFunction)kserial_serial_init, METH_VARARGS, "init serial connection" },
	{ "serial_recv", (PyCFunction)kserial_serial_recv, METH_VARARGS, "serial recev"},
	{ "serial_send", (PyCFunction)kserial_serial_send, METH_VARARGS, "serial send" },
	{ NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initkserial(void)	
{
	 (void) Py_InitModule("kserial", kserialMethods);
}

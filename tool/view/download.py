from kserial import *
from struct  import *
from re      import *
from PIL     import Image,ImageDraw
import os, array, sys

funcs = [""];
scale = 500;
colors = ["aqua","crimson","gold","pink","lime","maroon","navy","orange"];
left = 100;

def	get_func(name):
	i = 0;
	while i < len(funcs):
		if funcs[i] == name:
			return i;
		i += 1;
	funcs.insert(i, name);
	return i;

def	activity(draw,id,frm,to):
	x1 = 100 + frm / scale;
	x2 = 100 + to / scale;
	y = id * 15;

	draw.line([(x1, y), (x2, y)], fill=colors[id]);

def	state(draw,id,at,state):
	x = 100 + at / scale;
	y = id * 15;
	if state == -1:
		col = "green";
	if state == -2:
		col = "red";
	if state == -3:
		col = "blue";

	draw.line([(x - 5, y - 5), (x + 5, y + 5)], fill=col);
	draw.line([(x - 5, y + 5) , (x + 5, y - 5)], fill=col);

def     main():
	log = open("log", "w");

	serial_init(sys.argv[1]);
	serial_timeout(300000);
	cycle = serial_recv_cycle();

	img = Image.new("RGB", (cycle / scale + left, 200));
	draw = ImageDraw.Draw(img);
	draw.rectangle([(0, 0), (img.size[0], img.size[1])], fill="black");

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
			state(draw, func, t, cycle)
		else:
			log.write("+ " + tmp[1] + " executed for " + str(cycle) + "x10^3 cycles\n");
			activity(draw, func, t, t + cycle);
			t += cycle;
        log.close();

	i = 0;
	while i < len(funcs):
		x = draw.textsize(funcs[i], )[0];
		draw.text([(left - x - 10, (i * 15) - 5)], funcs[i], fill="white");
		i += 1;
	draw.line([(left, 0), (left, i * 15)], fill="white");
	draw.line([(left, i * 15), (img.size[0], i * 15)], fill="white");

	img.save("view.png");

main()

info: info.c gameutil.c gameutil.h inpututil.c inpututil.h
	gcc -o info info.c gameutil.c inpututil.c

change: change.c gameutil.c inpututil.c inpututil.h
	gcc -o change change.c gameutil.c inpututil.c 

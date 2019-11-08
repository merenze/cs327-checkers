all: info change

info: info.c gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	gcc -o info info.c gameutil.c inpututil.c movelist.c

change: change.c gameutil.c inpututil.c inpututil.h movelist.c movelist.h
	gcc -o change change.c gameutil.c inpututil.c movelist.c

hello: hello.c gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	gcc -o hello hello.c movelist.c gameutil.c inpututil.c


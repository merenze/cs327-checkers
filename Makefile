all: info change rankmoves edit

info: info.c gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	gcc -o info info.c gameutil.c inpututil.c movelist.c

change: change.c gameutil.c inpututil.c inpututil.h movelist.c movelist.h
	gcc -o change change.c gameutil.c inpututil.c movelist.c

rankmoves: rankmoves.c gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	gcc -o rankmoves rankmoves.c gameutil.c inpututil.c movelist.c

edit: edit.cpp gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	g++ -c edit.cpp
	g++ -o edit edit.o -ltermbox gameutil.c inpututil.c movelist.c

hello: hello.c gameutil.c gameutil.h inpututil.c inpututil.h movelist.c movelist.h
	gcc -o hello hello.c movelist.c gameutil.c inpututil.c

clean:
	rm -f info change rankmoves edit info.log rankmoves.log gameutil.log movelist.log *.o


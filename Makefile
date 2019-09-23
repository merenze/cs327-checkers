info: info.c gameutil.c inpututil.c
	gcc -o info info.c gameutil.c inpututil.c

inpututiltest: inpututiltest.c inpututil.c
	gcc -o inpututiltest inpututiltest.c inpututil.c


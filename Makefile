MAIN_HEADERS = bmp.h corr.h main.h config.h
MAIN_OBJECTS = bmp.o corr.o main.o

%.o: %.c $(MAIN_HEADERS)
	gcc -c $< -o $@

main: $(MAIN_OBJECTS)
	gcc $(MAIN_OBJECTS) -o $@

gmf: gmfinger.c
	gcc -L/home/zaf/thesis-apps/Share-Libs -Wall -o gmfinger gmfinger.c -lGMFinger

clean:
	rm -f main gmfinger *.o

cleans:
	rm -f assets/*Recons.bmp assets/*.txt

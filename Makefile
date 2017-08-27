MAIN_HEADERS = bmp.h corr.h config.h
MAIN_OBJECTS = bmp.o corr.o main.o

%.o: %.c $(MAIN_HEADERS)
	gcc -c $< -o $@

# compile program utama untuk rekonstruksi
# rekonstruksi dilakukan dalam 1 set image sekaligus
main: $(MAIN_OBJECTS)
	gcc $(MAIN_OBJECTS) -o $@

# compile program feature extraction dan matching sidik jari
# menggunakan library libGMFinger.so
gmf: gmfinger.c
	gcc -L/home/zaf/thesis-apps/Share-Libs -Wall -o gmfinger gmfinger.c -lGMFinger

match: match.c
	gcc -L/home/zaf/thesis-apps/Share-Libs -Wall -o match match.c -lGMFinger

# bersihkan executable
clean:
	rm -f main gmfinger match 4121 *.o

# bersihkan file dump lainnya
cleans:
	rm -f assets/*.bmp assets/*.txt

# jalankan program utama
run-main: clean main
	./main

# menjalankan test untuk mengetahui waktu komputasi
# run dijalankan sebanyak 10 kali untuk diambil waktu rata-rata
run-main-multi: clean main
	@for i in {1..10}; do ./main; sleep 1s; done

# jalankan program matching
run-gmf: clean gmf
	./gmfinger

run-match: clean match
	./matcher.sh

run-match-multi: clean match
	@for ii in {1..20}; do ./matcher.sh ${ii}; done

OBJ_4121 = bmp.o corr.o 4121.o
4121: $(OBJ_4121)
	gcc $(OBJ_4121) -o $@

run-4121: clean 4121
	./4121.sh
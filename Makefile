MAIN_HEADERS = bmp.h corr.h main.h config.h
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

# bersihkan executable
clean:
	rm -f main gmfinger *.o

# bersihkan file dump lainnya
cleans:
	rm -f assets/*.bmp assets/*.txt

# jalankan program utama
run-main: clean main
	./main

# menjalankan test untuk mengetahui waktu komputasi
# run dijalankan sebanyak 50 kali untuk diambil waktu rata-rata
run-main-multi: clean main
	@for i in {1..50}; do \
	./main; sleep 1s; \
	done

# jalankan program matching
run-gmf: clean gmf
	./gmfinger
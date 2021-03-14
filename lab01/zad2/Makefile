CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

all: static shared

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o

shared:
	$(CC) $(CFLAGS) -fPIC -c mylibrary.c -o mylibrary.o
	$(CC) $(CFLAGS) -shared -o libmylibrary.so mylibrary.o

clean:
	@rm -rf *.o *.a *.so main

tests:
	@echo ###### STATIC TESTS ######
	make -s test_static

	@echo ###### SHARED TESTS ######
	make -s test_shared


test_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main

	rm raport2.txt
	@echo ###### STATIC TESTS ###### >>
	make  --no-print-directory small >> raport2.txt
	make --no-print-directory medium >> raport2.txt
	make --no-print-directory large >> raport2.txt


test_shared: shared
	export LD_LIBRARY_PATH=$(DIR):$LD_LIBRARY_PATH
	$(CC) main.c -lmylibrary -L ./ -o main

	rm raport2.txt

	# --no-print-directory - to not print  “Entering/Leaving directory ...” messages
	make --no-print-directory small >> raport2.txt
	make --no-print-directory medium >> raport2.txt
	make --no-print-directory large >> raport2.txt


small:

	@echo "\n##### SMALL TESTS #####\n" >> raport2.txt
	@# @ makes silent
	@./main create_table 3 merge_files a.txt:b.txt c.txt:d.txt e.txt:f.txt \
	remove_block 2 remove_block 0 remove_block 1

	@echo "\n##### add remove repeat #####\n" >> raport2.txt
	@./main create_table 10 merge_files a.txt:b.txt c.txt:d.txt e.txt:f.txt \
    remove_row 2 100 remove_row 2 101 remove_row 2 120 remove_row 2 130 \
    merge_files a.txt:b.txt c.txt:d.txt e.txt:f.txt \
    remove_row 2 200 remove_row 2 22 remove_row 2 11 remove_row 2 7 \
    remove_block 1 2 remove_block 0 remove_block 1



# TODO: generowanie duzych
medium:
	@echo "\n##### MEDIUM TESTS #####\n" >> raport2.txt
	@# @ makes silent
	@./main create_table 5 merge_files a.txt:b.txt

large:
	@echo "\n##### LARGE TESTS #####\n" >> raport2.txt
	@# @ makes silent
	@./main create_table 5 merge_files a.txt:b.txt








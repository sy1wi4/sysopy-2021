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
	@rm -rf *.o *.a *.so file*.txt main

tests:
	make -s test_static

	make -s test_shared

	@#	make -s test_dynamic


test_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main

	@echo "~~~~~~~~~~~~~~~~~~~STATIC LIBRARIES TEST~~~~~~~~~~~~~~~~~~~" >> raport3a.txt
	make  --no-print-directory small >> raport3a.txt
	make --no-print-directory medium >> raport3a.txt
	make --no-print-directory large >> raport3a.txt


test_shared: shared
	export LD_LIBRARY_PATH=$(DIR):$LD_LIBRARY_PATH
	$(CC) main.c -lmylibrary -L ./ -o main

	@echo "~~~~~~~~~~~~~~~~~~~SHARED LIBRARIES TESTS~~~~~~~~~~~~~~~~~~~" >> raport3a.txt
	# --no-print-directory - to not print  “Entering/Leaving directory ...” messages
	make --no-print-directory small >> raport3a.txt
	make --no-print-directory medium >> raport3a.txt
	make --no-print-directory large >> raport3a.txt

test_dynamic: shared
	# TODO: :(
	@#	echo "~~~~~~~~~~~~~~~~~~~DYNAMIC LIBRARIES TESTS~~~~~~~~~~~~~~~~~~~" >> raport3a.txt


small:
	chmod +x gen.sh
	./gen.sh 6 10 file

	@echo "\n########### SMALL TESTS ###########\n" >> raport3a.txt
	@echo "6 files, 14 lines\n" >> raport3a.txt
	@# @ makes silent
	@./main create_table 10 merge_files \
	file0.txt:file1.txt file2.txt:file3.txt file4.txt:file5.txt \
	remove_row 0 0 remove_row 1 0 remove_row 2 1 remove_row 2 3 \
	remove_block 2 remove_block 0 remove_block 1

	@echo "\n########### SMALL add remove repeat ###########\n" >> raport3a.txt
	@./main create_table 10 merge_files \
	file0.txt:file1.txt file2.txt:file3.txt file4.txt:file5.txt \
	remove_row 0 0 remove_row 1 0 remove_row 2 1 remove_row 2 3 \
	remove_block 2 remove_block 0 remove_block 1 \
	merge_files \
    file0.txt:file1.txt file2.txt:file3.txt file4.txt:file5.txt \
	remove_row 0 0 remove_row 1 0 remove_row 2 1 remove_row 2 3 \
	remove_block 2 remove_block 0 remove_block 1 \
    merge_files \
    file0.txt:file1.txt file2.txt:file3.txt file4.txt:file5.txt \
	remove_row 0 0 remove_row 1 0 remove_row 2 1 remove_row 2 3 \
	remove_block 2 remove_block 0 remove_block 1


medium:
	chmod +x gen.sh
	./gen.sh 50 1000 file

	@echo "\n########### MEDIUM TESTS ###########\n" >> raport3a.txt
	@echo "50 files, 144 lines\n" >> raport3a.txt
	@# @ makes silent
	@./main create_table 1000 merge_files \
		 file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
         file15.txt:file16.txt \
         file16.txt:file17.txt \
         file17.txt:file18.txt \
         file18.txt:file19.txt \
         file19.txt:file20.txt \
         file20.txt:file21.txt \
         file21.txt:file22.txt \
         file22.txt:file23.txt \
         file23.txt:file24.txt \
         file24.txt:file25.txt \
         file25.txt:file26.txt \
         file26.txt:file27.txt \
         file27.txt:file28.txt \
         file28.txt:file29.txt \
         file29.txt:file30.txt \
         file30.txt:file31.txt \
         file31.txt:file32.txt \
         file32.txt:file33.txt \
         file33.txt:file34.txt \
         file34.txt:file35.txt \
         file35.txt:file36.txt \
         file36.txt:file37.txt \
         file37.txt:file38.txt \
         file38.txt:file39.txt \
         file39.txt:file40.txt \
         file40.txt:file41.txt \
         file41.txt:file42.txt \
         file42.txt:file43.txt \
         file43.txt:file44.txt \
         file44.txt:file45.txt \
         file45.txt:file46.txt \
         file46.txt:file47.txt \
         file47.txt:file48.txt \
         file48.txt:file49.txt \
			remove_row 0 0 \
        	remove_row 0 0 \
        	remove_row 1 0 \
        	remove_row 1 0 \
        	remove_row 2 0 \
        	remove_row 2 0 \
        	remove_row 3 0 \
        	remove_row 3 0 \
        	remove_row 4 0 \
        	remove_row 4 0 \
        	remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7

	@echo "\n########### MEDIUM add remove repeat ###########\n" >> raport3a.txt
	@./main create_table 1000 merge_files \
		file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14 \
        	merge_files file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14 \
		merge_files file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14

large:

	chmod +x gen.sh
	./gen.sh 100 800000 file

	@echo "\n########### LARGE TESTS ###########\n" >> raport3a.txt
	@echo "100 files, 10400 lines\n" >> raport3a.txt
	@# @ makes silent
	@./main create_table 1000 merge_files \
		 file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
         file15.txt:file16.txt \
         file16.txt:file17.txt \
         file17.txt:file18.txt \
         file18.txt:file19.txt \
         file19.txt:file20.txt \
         file20.txt:file21.txt \
         file21.txt:file22.txt \
         file22.txt:file23.txt \
         file23.txt:file24.txt \
         file24.txt:file25.txt \
         file25.txt:file26.txt \
         file26.txt:file27.txt \
         file27.txt:file28.txt \
         file28.txt:file29.txt \
         file29.txt:file30.txt \
         file30.txt:file31.txt \
         file31.txt:file32.txt \
         file32.txt:file33.txt \
         file33.txt:file34.txt \
         file34.txt:file35.txt \
         file35.txt:file36.txt \
         file36.txt:file37.txt \
         file37.txt:file38.txt \
         file38.txt:file39.txt \
         file39.txt:file40.txt \
         file40.txt:file41.txt \
         file41.txt:file42.txt \
         file42.txt:file43.txt \
         file43.txt:file44.txt \
         file44.txt:file45.txt \
         file45.txt:file46.txt \
         file46.txt:file47.txt \
         file47.txt:file48.txt \
         file48.txt:file49.txt \
         file49.txt:file50.txt \
         file50.txt:file51.txt \
         file51.txt:file52.txt \
         file52.txt:file53.txt \
         file53.txt:file54.txt \
         file54.txt:file55.txt \
         file55.txt:file56.txt \
         file56.txt:file57.txt \
         file57.txt:file58.txt \
         file58.txt:file59.txt \
         file59.txt:file60.txt \
         file60.txt:file61.txt \
         file61.txt:file62.txt \
         file62.txt:file63.txt \
         file63.txt:file64.txt \
         file64.txt:file65.txt \
         file65.txt:file66.txt \
         file66.txt:file67.txt \
         file67.txt:file68.txt \
         file68.txt:file69.txt \
         file69.txt:file70.txt \
         file70.txt:file71.txt \
         file71.txt:file72.txt \
         file72.txt:file73.txt \
         file73.txt:file74.txt \
         file74.txt:file75.txt \
         file75.txt:file76.txt \
         file76.txt:file77.txt \
         file77.txt:file78.txt \
         file78.txt:file79.txt \
         file79.txt:file80.txt \
         file80.txt:file81.txt \
         file81.txt:file82.txt \
         file82.txt:file83.txt \
         file83.txt:file84.txt \
         file84.txt:file85.txt \
         file85.txt:file86.txt \
         file86.txt:file87.txt \
         file87.txt:file88.txt \
         file88.txt:file89.txt \
         file89.txt:file90.txt \
         file90.txt:file91.txt \
         file91.txt:file92.txt \
         file92.txt:file93.txt \
         file93.txt:file94.txt \
         file94.txt:file95.txt \
         file95.txt:file96.txt \
         file96.txt:file97.txt \
         file97.txt:file98.txt \
         file98.txt:file99.txt \
			remove_row 0 0 \
        	remove_row 0 0 \
        	remove_row 1 0 \
        	remove_row 1 0 \
        	remove_row 2 0 \
        	remove_row 2 0 \
        	remove_row 3 0 \
        	remove_row 3 0 \
        	remove_row 4 0 \
        	remove_row 4 0 \
        	remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7

	@echo "\n########### LARGE add remove repeat ###########\n" >> raport3a.txt
	@./main create_table 1000 merge_files \
		file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14 \
        	merge_files file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14 \
		merge_files file0.txt:file1.txt \
         file1.txt:file2.txt \
         file2.txt:file3.txt \
         file3.txt:file4.txt \
         file4.txt:file5.txt \
         file5.txt:file6.txt \
         file6.txt:file7.txt \
         file7.txt:file8.txt \
         file8.txt:file9.txt \
         file9.txt:file10.txt \
         file10.txt:file11.txt \
         file11.txt:file12.txt \
         file12.txt:file13.txt \
         file13.txt:file14.txt \
         file14.txt:file15.txt \
			remove_block 0 \
        	remove_block 2 \
        	remove_block 1 \
        	remove_block 2 \
        	remove_block 3 \
        	remove_block 4 \
        	remove_block 5 \
        	remove_block 6 \
        	remove_block 7 \
			remove_block 8 \
        	remove_block 9 \
        	remove_block 10 \
        	remove_block 11 \
        	remove_block 12 \
        	remove_block 13 \
        	remove_block 14






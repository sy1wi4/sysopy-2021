CC = gcc
CFLAGS = -Wall -g
DIR := ${CURDIR}

all: run_static

static:
	$(CC) $(CFLAGS) -c mylibrary.c -o mylibrary.o
	ar rcs libmylibrary.a mylibrary.o


run_static: static
	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main
	./main $(ARGS)

clean:
	@rm -rf *.o *.a *.so temp.txt file*.txt f main

tests: static
	rm -f raport.txt


	$(CC) -c main.c
	$(CC) main.o libmylibrary.a -o main

	@echo "~~~~~~~~~~~~~~~~~~~TEST~~~~~~~~~~~~~~~~~~~" >> raport.txt
	make  --no-print-directory small >> raport.txt
	make --no-print-directory medium >> raport.txt
	make --no-print-directory large >> raport.txt

small:
	@chmod +x gen.sh
	@./gen.sh 6 10 file

	@echo "\n########### SMALL TESTS ###########\n" >> raport.txt
	@echo "6 files, 14 lines\n" >> raport.txt
	@./main merge_files file0.txt:file1.txt file2.txt:file3.txt file4.txt:file5.txt

medium:
	@chmod +x gen.sh
	@./gen.sh 50 1000 file

	@echo "\n########### MEDIUM TESTS ###########\n" >> raport.txt
	@echo "50 files, 274 lines\n" >> raport.txt
	@./main create_table 20000 merge_files \
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

large:

	@chmod +x gen.sh
	@./gen.sh 100 800000 file

	@echo "\n########### LARGE TESTS ###########\n" >> raport.txt
	@echo "100 files, 10400  lines\n" >> raport.txt
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

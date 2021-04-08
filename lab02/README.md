We wszystkich poniższych zadaniach proszę skorzystać z obu wariantów implementacji:

lib - przy użyciu funkcji biblioteki C: fread() i fwrite()
sys - przy użyciu funkcji systemowych: read() i write()

## Zadanie 1 (20%) 
Napisz program, który otwiera dwa pliki o nazwach podanych w wierszu poleceń. Jeśli argumentów nie podano, wówczas nazwy plików mają być pobrane od użytkownika. Program powinien wyświetlać wiersze z obu plików naprzemienne, to znaczy: 1-szą linię z pierwszego pliku, 1-szą linię z drugiego pliku, 2-gą linię z pierwszego pliku, 2-gą linię z drugiego pliku, itd., aż do momentu, wyświetlenia ostatniego wiersza pliku zawierającego większą liczbę wierszy.

## Zadanie 2 (20%) 
Napisz program, który przyjmuje 2 argumenty wiersza poleceń. Pierwszy z argumentów jest znakiem, drugi nazwą pliku. Program powinien wyświetlić na ekranie tylko te wiersze pliku wejściowego,które zawierają dany znak. Zakładamy, że każdy wiersz w pliku kończy się znakiem przejścia do nowej linii. Przyjmujemy, że żaden wiersz nie przekracza długości 256 znaków.

## Zadanie 3 (20%) 
W pliku dane.txt znajdują się w kolejnych wierszach losowe liczby.Do pliku a.txt wpisz ilość liczb parzystych znajdujących się w pliku dane.txt w następującej postaci: ”Liczb parzystych jest [ilość liczb]”. Do pliku b.txt skopiuj wszystkie liczby z pliku dane.txt, w których cyfra dziesiątek jest równa 7 lub 0.Do pliku c.txt skopiuj wszystkie liczby, które są kwadratami liczb całkowitych, np. taką liczbą jest liczba 225, ponieważ 225 = 15^2.

## Zadanie 4 (20%) 
Napisz funkcję, która jako parametry pobiera nazwę pliku do odczytu, nazwę pliku do zapisu oraz 2 napisy,n1 oraz n2(tablice znaków). Zadaniem funkcji jest przepisanie pliku wejściowego do wyjściowego w taki sposób, że każde wystąpienie napisu n1 w pliku wejściowym ma zostać zamienione na napis n2 w pliku wyjściowym.

## Zadanie 5 (20%) 
Napisz program, który kopiując podany plik (parametr programu) do innego pliku (drugi parametr programu), ”łamie” wiersze, które mają więcej niż 50 znaków (łącznie ze spacjami). Znaki po-wyżej 50-tego przenoszone są do nowej linii (dodatkowy wiersz). Wiersze krótsze kopiowane są bez zmian

Dla obu wariantów implementacji należy przeprowadzić pomiar czasu wykonywania obu wariantów programów. Wyniki należy przedstawić w formie pliku pomiar_zad_x.txt

## Zadanie 1 (20%)
Napisz program, w którym z procesu macierzystego stworzysz dokładnie n procesów potomnych (n parametr wywołania programu). W każdym z procesów potomnych wypisz komunikat, w którym poinformujesz z którego procesu napis pochodzi.

## Zadanie 2 (40%)
Korzystając z biblioteki z zadania 1 zestawu 1 napisz program do mergowania n par plików  w taki sposób, iż każda para plików mergowana jest w osobnym procesie potomnym. Przeprowadź pomiary czasu operacji mergowania analogiczne do tych z zadania 2 zestawu pierwszego, a zatem  dla różnej ilości par (mała, średnia oraz duża ilość par) dla różnego stopnia wielkości wierszy w plikach w parze (pliki z dużą ilością wierszy, pliki ze średnią ilością wierszy, pliki z małą ilością wierszy). Porównaj otrzymane wyniki z tymi uzyskanymi przy okazji rozwiązywania zadań z zestawu pierwszego.

## Zadanie 3 (40%)
Napisz program, który rozpoczynając od katalogu podanego jako pierwszy parametr uruchomienia, idąc w głąb drzewa katalogów, znajdzie pliki zawierające łańcuch podany jako drugi parametr uruchomienia programu. Przeszukiwanie każdego z podkatalogów powinno odbyć się w osobnym procesie potomnym. Wydruk wyniku wyszukiwania poprzedź wypisaniem ścieżki względnej od katalogu podanego jako argument uruchomienia oraz numeru PID procesu odpowiedzialnego za przeglądanie określonego (pod)katalogu. Przeszukiwanie powinno obejmować pliki tekstowe i pomijać pliki binarne/wykonywalne/obiektowe etc. Program jako trzeci parametr powinien przyjmować maksymalną głębokość przeszukiwania licząc od katalogu podanego jako pierwszy parametr.

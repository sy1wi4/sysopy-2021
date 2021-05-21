## Zadania - Zestaw 9
### Opis problemu:
W ramach zadania należy zaimplementować rozwiązanie problemu Świętego Mikołaja.
Święty Mikołaj śpi w swoim warsztacie na biegunie północnym i może być obudzony tylko w 2 sytuacjach:
1) wszystkie 9 reniferów wróciło z wakacji,
2) 3 z 10 elfów ma problemy przy produkcji zabawek.

Kiedy problemy 3 elfów są rozwiązywane przez Mikołaja to pozostałe nie zgłaszają swoich problemów aż do powrotu pierwszej trójki.

Jeśli Mikołaj się obudzi i zastanie jednocześnie 9 reniferów i 3 elfy przed swoim warsztatem to stwierdza że problemy elfów mogą poczekać i ważniejsze jest rozwiezienie prezentów.

Należy zaimplementować program, w którym Mikołaj, renifery oraz elfy to osobne wątki.

Zachowania elfów:

* Pracują przez losowy okres czasu (2-5s).
* Chcą zgłosić problem - jeśli liczba oczekujących elfów przed warsztatem Mikołaja jest mniejsza od 3 to idzie przed warsztat (Komunikat: Elf: czeka _ elfów na Mikołaja, ID), w przeciwnym wypadku czeka na powrót pierwszej trójki i dopiero wtedy idzie przed warsztat (Komunikat: Elf: czeka na powrót elfów, ID)
* Jeśli jest trzecim elfem przed warsztatem to wybudza Mikołaja. (Komunikat: Elf: wybudzam Mikołaja, ID)
* Mikołaj się z nimi spotyka. (Komunikat: Elf: Mikołaj rozwiązuje problem, ID) (1-2s)
* Wraca do pracy

Zachowania reniferów:

* Są na wakacjach w ciepłych krajach losowy okres czasu (5-10s)
* Wracaja na biegun północny (Komunikat: Renifer: czeka _ reniferów na Mikołaja, ID), jeśli jest dziewiątym reniferem to wybudza Mikołaja (Komunikat: Renifer: wybudzam Mikołaja, ID).
* Dostarczają zabawki grzecznym dzieciom (i studentom którzy nie spóźniają się z dostarczaniem zestawów) przez (2-4s).   
* Lecą na wakacje.

Zachowania Mikołaja:

* Śpi.
* Kiedy zostaje wybudzony (Komunikat: Mikołaj: budzę się):
  * i jest 9 reniferów to dostarcza zabawki (Komunikat: Mikołaj: dostarczam zabawki) (2-4s).
  * i są 3 elfy to bierze je do swojego warsztatu i rozwiązuje problemy (Komunikat: Mikołaj: rozwiązuje problemy elfów _ _ _ ID) (1-2). 
* Wraca do snu (Komunikat: Mikołaj: zasypiam).
  
Program należy zaimplementować korzystając z wątków i mechanizmów synchronizacji biblioteki POSIX Threads. Po uruchomieniu programu wątek główny tworzy wątki dla Mikołaja, reniferów oraz elfów. Możemy założyć że Mikołaj dostarczy 3 razy prezenty, po czym kończy działanie wszystkich wątków. Do spania Mikołaja powinny być wykorzystane Warunki Sprawdzające (Condition Variables). Użycie odpowiednich mechanizmów ma zagwarantować niedopouszczenie, np. do zdarzeń:

* Mikołaj śpi chociaż czeka na niego 9 reniferów lub 3 elfy.
* Na Mikołaja czeka więcej niż 3 elfy.

Pełne rozwiązanie zadania - 100%. Wersja uproszczona - (Mikołaj i renifery) lub (Mikołaj i elfy) - 60%.
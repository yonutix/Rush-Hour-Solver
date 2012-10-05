IDE: VIsual Studio 2010
SO: Windows 7


Se poate afisa timpul prin decomentarea liniei 10 din lib.h

Am testat pe un Intel i3 2.26 GHz, rezolva exemplul de pe forum in modul Debug in 25s,
in modul Release in 1.2s,
iar daca este rulat separat executabilul in cel mult 0.5s.

Am facut BFS cu restrictia ca in doua state-uri consecutive nu poate fi mutata aceeasi masina.
Am optimizat cautarea in vectorul de stari vizitate facand-o binar.
Cautarea este legata si de inserarea in vector, astfel ca daca nu este gasit elementul este si inserat ca sa nu se mai faca din nou o cautare sau eventual o sortare.
Am definit relatii de ordine pe multimea starilor si a masinilor pentru a putea cauta binar.O stare este mai mare decat alta stare daca pe o pozitie cat mai semnicifcativa se afla o masina mai mare, este mai mica daca pe o pozitie cat mai semnificativa se afla o masina mai mica.Daca toate masinile sunt pe aceleasi pozitii starile sunt egale.
Masinile sunt ordonate intai dupa coordonata x apoi dupa y.

Fac urmatoarea presupunere: iesirea este doar pe partea dreapta pe orice linie.

P.S.
Scuze pentru memory leak.
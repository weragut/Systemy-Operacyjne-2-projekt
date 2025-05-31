# Systemy Operacyjne - Projekty C++
---

# Projekt 1: Problem jedzących filozofów

Ten projekt jest symulacją problemu jedzących filozofów, zaimplementowaną w C++ z wykorzystaniem mechanizmów synchronizacji: semafory i mutexy.

## Opis problemu jedzących filozofów (dining philosophers problem)
Problem jedzących filozofów to synchronizacyjny problem, który naświetla trudności zarządzaniem dostępem do wspólnych zasobów. Jest to problem opisujący określoną liczbę filozofów siedzących przy okrągłym stole i taką samą liczbę widelców. Widelec leży między każdą parą sąsiadujących filozofów, więc każdy z nich ma po jednym widelcu po lewej i prawej stronie. Filozofowie mogą znajdować się w trzech stanach: myślący, głodny i jedzący. Aby zjeść filozof potrzebuje dwóch widelców, lecz może on sięgnąć tylko po te znajdujące się po jego lewej lub prawej stronie. Problem pojawia się, gdy kilku filozofów staje się głodnych jednocześnie i próbują sięgnąć po dostępne widelce, co może prowadzić do zakleszczenia – sytuacji, w której żaden z filozofów nie jest w stanie zakończyć swojej aktywności. Rozwiązanie tego problemu wymaga zastosowania odpowiednich mechanizmów synchronizacji, które temu zapobiegają oraz gwarantują, że każdy filozof może w odpowiednim momencie zjeść i powrócić do myślenia.

## Rozwiązanie Dijkstry
Rozwiązanie Dijkstry polega założeniu, że filozofowie zawsze sięgają po oba sąsiadujące z nimi widelce jednocześnie, eliminując tym samym możliwość trzymania tylko jednego widelca w ręku i oczekiwaniana drugi.  Aby to zrealizować, w rozwiązaniu Dijkstry każdemu filozofowi przypisuje się semafor, który kontroluje dostęp do widelców, oraz mutex, który synchronizuje dostęp do sekcji krytycznych, takich jak zmiana stanu filozofa. Każdy filozof sprawdza, czy żaden z jego sąsiadów nie je i dopiero po tym on sam może przejść do stanu jedzenia. Po skończonym jedzeniu, filozof odkłada widelce, zmieniając swój stan na myślenie, a następnie sprawdza, czy jego sąsiedzi mogą zacząć jeść.

## Wątki
W projekcie każdy filozof jest reprezentowany przez **osobny wątek**. Wątek ten wykonuje cyklicznie cztery działania: myślenie, branie widelców, jedzenie i odkładanie widelców. Każdy filozof wykonuje te operacje w nieskończonej pętli, aby symulować ciągłe przeplatanie się myślenia i jedzenia. Każdy wątek filozofa działa w sposób niezależny, co sprawia, że procesy są współbieżne. Dzięki temu możliwe jest jednoczesne wykonywanie działań przez wielu filozofów, jednocześnie dbając o to, aby dostęp do wspólnych zasobów (widelców) był odpowiednio synchronizowany.

## Sekcje krytyczne
Sekcje krytyczne to fragmenty kodu, gdzie dochodzi do zmiany stanu filozofów lub dostępu do wspólnych zasobów (widelców). Aby uniknąć race conditions (wyścigu wątków), w aplikacji wykorzystano **mutexy** i **semafory**.
### Mutexy:
- **critical_region_mtx** - Służy do ochrony sekcji kodu, w której filozofowie sprawdzają dostępność widelców oraz zmieniają swoje stany. Zapewnia to, że tylko jeden filozof w danym momencie zmienia swój stan, zapobiegając jednoczesnej modyfikacji stanu przez wielu filozofów.
- **cout_mtx** - Synchronizuje wypisanie stanu filozofa na konsoli, aby uniknąć mieszania się komunikatów między różnymi wątkami.
### Semafory
- **both_forks_available** - Dla każdego filozofa przypisany jest semafor binarny, który kontroluje dostęp do dwóch widelców obok niego. Filozof może rozpocząć jedzenie tylko wtedy, gdy oba widelce są dostępne. 

## Instrukcja uruchomienia projektu
### Wymagania

- **Kompilator C++** wspierający C++20 
- **Terminal**

### Klonowanie repozytorium

Za pomocą poniższej komendy sklonuj repozytorium 

```bash
git clone https://github.com/weragut/Systemy-Operacyjne-2-projekt.git
```
Następnie należy przejść do folderu projektu komendą:
```bash
cd Systemy-Operacyjne-2-projekt
```

### Budowanie projektu
Skorzystaj z poniższej komendy, aby zbudować projekt:

```bash
g++ -std=c++20 -o SO.exe philosophers/main.cpp philosophers/DiningPhilosophers.cpp

```

Po skompilowaniu projektu, uruchom go za pomocą poniższej komendy, podając jako argument pożądaną liczbę filozofów.

```bash
./SO 6
```

---

# Projekt 2: Wielowątkowy serwer czatu

### Opis projektu 

Prosty, wielowątkowy chat sieciowy napisany w C++ z wykorzystaniem socketów do komunikacji. Projekt składa się z dwóch oddzielnych aplikacji:

- **chat_server** — serwer, który zarządza wieloma połączeniami klientów, odbiera od nich wiadomości i rozsyła je do wszystkich podłączonych użytkowników. Serwer pełni funkcję centralnego punktu komunikacji.

- **chat_client** — klient łączący się z serwerem, umożliwiający użytkownikowi wysyłanie wiadomości oraz odbieranie ich od innych uczestników czatu w czasie rzeczywistym.

Serwer obsługuje wielu klientów jednocześnie, wykorzystując wielowątkowość, co pozwala na asynchroniczną komunikację oraz zapewnia płynne działanie chatu. Projekt na systemach Windows wymaga biblioteki ws2_32.

## Wątki
Projekt wykorzystuje wielowątkowość, aby zapewnić równoczesną i asynchroniczną komunikację pomiędzy wieloma klientami oraz serwerem. 

- **Wątek główny serwera** :  
  Nasłuchuje na połączenia przychodzące od klientów za pomocą funkcji `accept()`. Gdy nowy klient się połączy, serwer tworzy dla niego osobny wątek, który zajmuje się dalszą komunikacją z tym klientem. Pozwala to na obsługę wielu klientów jednocześnie bez blokowania głównego wątku serwera.

- **Wątki klientów na serwerze**:  
  Każdy z nich odbiera wiadomości od przypisanego klienta (`recv()`), a następnie wysyła je do pozostałych klientów. Wątki działają równolegle i niezależnie, co umożliwia asynchroniczną obsługę komunikacji.

- **Wątek odbierający wiadomości po stronie klienta**:  
  Ten wątek działa równolegle z główną pętlą wysyłania wiadomości w kliencie. Odbiera na bieżąco wiadomości z serwera (`recv()`), wyświetlając je w konsoli, dzięki czemu użytkownik może jednocześnie wysyłać i odbierać wiadomości bez blokad.


## Sekcje krytyczne

W serwerze czatu działają równocześnie wątki, które współdzielą zasoby, takie jak lista aktywnych klientów oraz konsola służąca do wyświetlania komunikatów. Aby uniknąć błędów związanych z jednoczesnym dostępem wielu wątków do tych zasobów (tzw. race conditions), zastosowano mechanizmy synchronizacji, które chronią sekcje krytyczne.

### Mutexy:
- **clients_mutex** - Chroni dostęp do wspólnej listy `active_clients`, która zawiera sockety i identyfikatory aktywnych klientów. Zapewnia bezpieczne dodawanie, usuwanie klientów.
 
### Semafory
- **output_mutex** - Używany do synchronizacji wypisywania informacji na konsolę (np. przychodzących wiadomości, logów połączeń i rozłączeń). Zapobiega mieszaniu się tekstu wypisywanego równocześnie przez wiele wątków.


## Instrukcja uruchomienia projektu
### Wymagania

- **Kompilator C++** wspierający C++20 
- **Terminal**

### Klonowanie repozytorium

Za pomocą poniższej komendy sklonuj repozytorium 

```bash
git clone https://github.com/weragut/Systemy-Operacyjne-2-projekt.git
```
Następnie należy przejść do folderu projektu komendą:
```bash
cd Systemy-Operacyjne-2-projekt
```

### Budowanie projektu
Skorzystaj z poniższych komend, aby zbudować aplikacje serwera i klienta:

Budowa aplikacji serwera:
```bash
g++ -std=c++20 -o chat_server chat/server.cpp -lws2_32
```
Budowa aplikacji klienta:
```bash
g++ -std=c++20 -o chat_client chat/client.cpp -lws2_32
```
### Uruchamianie serwera i klientów
W osobnych terminalach z poziomu katalogu Systemy-Operacyjne-2-projekt.
1. Uruchom serwer czatu:
```bash
./chat_server
```
Serwer uruchomi się lokalnie na porcie 54000 i będzie oczekiwał na połączenia od klientów.

2. Uruchom klienta
   
Każdego klienta uruchom w osobnym terminalu z poziomu katalogu Systemy-Operacyjne-2-projekt.
```bash
./chat_client
```
Każdy klient połączy się z lokalnym serwerem i będzie mógł przesyłać oraz odbierać wiadomości w czasie rzeczywistym.

3. Zakończenie sesji

Aby zakończyć połączenie klienta z serwerem, wpisz w konsoli klienta:
```bash
/exit
```

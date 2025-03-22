# Problem jedzących filozofów

Ten projekt jest symulacją problemu jedzących filozofów, zaimplementowaną w C++ z wykorzystaniem mechanizmów synchronizacji: semafory i mutexy.

## Wymagania

- **Kompilator C++** wspierający C++20 
- **Terminal** lub **wiersz poleceń** w systemie operacyjnym

## Instrukcja uruchomienia projektu

### Klonowanie repozytorium

Sklonuj sobie

```bash
https://github.com/weragut/Systemy-Operacyjne-2-projekt.git
cd cmake-build-debug
```

### Budowanie projektu
Skorzystaj z poniższej komendy, aby zbudować projekt:

```bash
g++ -std=c++20 -o SO.exe main.cpp DiningPhilosophers.cpp
```
Po skompilowaniu projektu, uruchom go za pomocą poniższej komendy, podając jako argument pożądaną liczbę filozofów.

```bash
./SO 6
```




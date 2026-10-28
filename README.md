# ⚡ Technika Cyfrowa i Mikroprocesorowa

Rozwiązania zadań laboratoryjnych z kursu **Technika Cyfrowa i Mikroprocesorowa (TCiM)**, realizowanego na kierunku *Informatyczne Systemy Automatyki* na Politechnice Wrocławskiej.

> 🇬🇧 English version: [README.en.md](README.en.md)

## 📌 O repozytorium

Repozytorium zawiera programy na mikrokontroler (Arduino, C/C++) obsługujące diody LED i przyciski, schemat układu laboratoryjnego zaprojektowany w KiCad oraz rozwiązania zadań z kolejnych laboratoriów.

## 🧪 Zawartość

### Lab1 – wejścia/wyjścia cyfrowe (Arduino + KiCad)

| Szkic | Opis |
|---|---|
| `miganie_diodami` | Sterowanie diodami LED – miganie z zadanym okresem |
| `walking_point` | Efekt „biegnącego punktu” na linijce diod |
| `przycisk_mono` | Przycisk działający monostabilnie – dioda świeci tylko podczas wciśnięcia |
| `przycisk_bi` | Przycisk działający bistabilnie – każde wciśnięcie przełącza stan diody |
| `dwa_przyciski` | Obsługa dwóch przycisków sterujących wyjściami |
| `cztery_przyciski` | Obsługa czterech przycisków sterujących wyjściami |

`schemat_lab1` – projekt KiCad (schemat ideowy i PCB) układu użytego w laboratorium, wraz z eksportem do PNG.

### Lab2

Rozwiązania zadań 3.2, 4.1 i 4.2 z instrukcji laboratoryjnej.

## 🛠️ Technologie

- Arduino (C/C++), Arduino IDE
- KiCad – schematy i projekt PCB

## 🚀 Uruchomienie

1. Otwórz wybrany plik `.ino` w Arduino IDE.
2. Wybierz płytkę i port w menu *Narzędzia*.
3. Wgraj program na płytkę.

Projekt schematu otwiera się w KiCad przez plik `schemat_lab1.kicad_pro`.

## 👤 Autor

Franciszek Pora – Politechnika Wrocławska, Informatyczne Systemy Automatyki

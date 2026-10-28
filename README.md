# ⚡ Technika Cyfrowa i Mikroprocesorowa

Rozwiązania zadań laboratoryjnych z kursu **Technika Cyfrowa i Mikroprocesorowa (TCiM)**, realizowanego na kierunku *Informatyczne Systemy Automatyki* na Politechnice Wrocławskiej.

> 🇬🇧 English version: [README.en.md](README.en.md)

## 📌 O repozytorium

Repozytorium zawiera programy na mikrokontroler **AVR ATmega328P** (Arduino Nano) pisane w trakcie kolejnych laboratoriów. Kurs prowadził od obsługi pojedynczych pinów do kompletnych układów pomiarowo-sterujących.

Większość zadań (Lab1–Lab5 oraz laboratorium zaliczeniowe) jest napisana w **czystym C na rejestrach mikrokontrolera**, bez warstwy Arduino: porty `DDRx/PORTx/PINx`, timery, przerwania, przetwornik ADC i USART są konfigurowane bezpośrednio przez bity rejestrów. Ostatnie laboratoria (Lab6, Lab7, lab dodatkowy) korzystają z frameworka Arduino i bibliotek do obsługi czujników, pilota IR i wyświetlaczy.

## 🧪 Zakres laboratoriów

| Folder | Temat | Co zostało zrealizowane |
|---|---|---|
| `Lab1` | Wejścia i wyjścia cyfrowe | Miganie diodami, „biegnący punkt”, przycisk monostabilny i bistabilny, eliminacja drgań styków, sterowanie linijką LED czterema przyciskami. Schemat układu w KiCad (`schemat_lab1`). |
| `Lab2` | Wyświetlacz siedmiosegmentowy i buzzer | Tablica kodów cyfr, multipleksowanie czterech cyfr, sygnalizacja dźwiękowa, minutnik z odliczaniem i alarmem. |
| `Lab3` | Timery i przerwania | Timer1 w trybie CTC, przerwanie zewnętrzne INT0, multipleksowanie wyświetlacza w przerwaniu Timer0, obsługa enkodera obrotowego (INT0 i Pin Change Interrupt) z licznikiem w zakresie −100…100 i znakiem minus. |
| `Lab4` | LCD przez I2C i przetwornik ADC | Licznik sterowany przyciskami, zegar z budzikiem na przerwaniu Timer1, woltomierz na ADC, włącznik zmierzchowy z histerezą (potencjometr i fotorezystor). |
| `Lab5` | Komunikacja szeregowa USART | Własne funkcje nadawania i odbioru, przesyłanie pomiarów ADC do Kreślarki Arduino, sterowanie układem komendami z terminala, kalkulator wieku wczytujący liczbę z konsoli. |
| `Lab6` | Framework Arduino, czujnik temperatury | Pomiar napięcia przez `analogRead`, termometr DS18B20 (1-Wire) z wyświetlaniem na LCD, sterowanie diodą RGB przez PWM, wskaźnik nagrzewania. |
| `Lab7` | Pilot IR i termostat | Odczyt kodów pilota podczerwieni, termostat sterowany pilotem (temperatura zadana, tryb czuwania), rozszerzenie o blokadę pracy przy zbyt słabym oświetleniu. |
| `labDodatkowy` | Wyświetlacz OLED | Bitmapy i tekst na wyświetlaczu OLED SH1106 (I2C), animacja spadających grafik – kartka wielkanocna. |
| `Lab_zaliczenie` | Laboratorium zaliczeniowe | Wielozadaniowość bez blokujących opóźnień, zegar i odliczanie na przerwaniach, pomiar ADC na żądanie, system pomiarowy sterowany komendami UART (odbiór w przerwaniu, tryb jednorazowy i cykliczny, wybór kanałów). |
| `LCD_I2C` | Biblioteka LCD | Obsługa magistrali I2C, ekspandera PCF8574 i wyświetlacza HD44780 wraz z programem demonstracyjnym. |

## ✨ Wybrane rozwiązania

- **Enkoder obrotowy z wyświetlaczem** (`Lab3/zad5_2`) – program działa wyłącznie na przerwaniach: Timer0 odświeża wyświetlacz, a przerwanie Pin Change odczytuje kierunek obrotu. Pętla główna jest pusta.
- **Zegar z budzikiem** (`Lab4/Zad2`) – odmierzanie czasu przerwaniem Timer1 w trybie CTC, ustawianie alarmu przyciskami, wyświetlanie na LCD przez I2C.
- **Włącznik zmierzchowy z histerezą** (`Lab4/Zad4`, `Lab5/Zad2_2`) – porównanie odczytów dwóch kanałów ADC, a w wersji z Lab5 również wykres przebiegów w Kreślarce.
- **Termostat sterowany pilotem** (`Lab7/Zad4`) – czujnik DS18B20, odbiornik IR, fotorezystor i LCD w jednym układzie.
- **System pomiarowy przez UART** (`Lab_zaliczenie/zad5`) – interpreter komend tekstowych (`jeden`, `cykliczny-X`, `stop`, `CH-X-Y`) z buforem odbiorczym wypełnianym w przerwaniu.

## 🛠️ Technologie

- C/C++ (AVR-libc), framework Arduino, Arduino IDE
- Mikrokontroler ATmega328P (Arduino Nano), 16 MHz
- Peryferia: GPIO, Timer0/Timer1, przerwania zewnętrzne i Pin Change, ADC, USART, I2C
- Elementy: diody LED i RGB, przyciski, buzzer, wyświetlacz siedmiosegmentowy, LCD HD44780 z ekspanderem PCF8574, OLED SH1106, enkoder, potencjometr, fotorezystor, czujnik DS18B20, odbiornik podczerwieni
- KiCad – schemat i projekt PCB

## 🚀 Uruchomienie

1. Otwórz wybrany plik `.ino` w Arduino IDE. Pliki `.cpp` i `.h` z tego samego folderu zostaną dołączone automatycznie.
2. Wybierz płytkę **Arduino Nano** i odpowiedni port.
3. Wgraj program na płytkę.

Programy z Lab6, Lab7 i labu dodatkowego wymagają bibliotek z menedżera bibliotek Arduino IDE: `OneWire`, `DallasTemperature`, `LiquidCrystal_I2C`, `IRremote`, `Adafruit SH110X` (wraz z `Adafruit GFX`).

Programy korzystające z USART komunikują się z prędkością 19200 bit/s, a zadanie 5 z laboratorium zaliczeniowego z prędkością 9600 bit/s.

## 📁 Uwagi

Biblioteka w folderach `LCD_I2C` (pliki `I2C`, `PCF8574`, `LCDI2C`) pochodzi z materiałów udostępnionych na zajęciach. Autor jest podany w nagłówkach plików.

## 👤 Autor

Franciszek Pora – Politechnika Wrocławska, Informatyczne Systemy Automatyki

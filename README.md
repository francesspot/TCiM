🇵🇱 **Polski** | 🇬🇧 [English](README.en.md)

# ⚡ Technika Cyfrowa i Mikroprocesorowa — laboratoria

![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-00878F?logo=arduino&logoColor=white) ![AVR](https://img.shields.io/badge/AVR_ATmega328P-E51B23) ![KiCad](https://img.shields.io/badge/KiCad-314CB0?logo=kicad&logoColor=white)

> 🎓 Zadania laboratoryjne z przedmiotu **Technika Cyfrowa i Mikroprocesorowa** (Politechnika Wrocławska)

Programy na mikrokontroler AVR ATmega328P (Arduino Nano) — od migania diodą po układy pomiarowo-sterujące. Większość zadań napisałem w C bezpośrednio na rejestrach mikrokontrolera, bez warstwy Arduino: porty, timery, przerwania, ADC i USART konfigurowałem bit po bicie. Dopiero ostatnie laboratoria korzystają z frameworka Arduino i gotowych bibliotek do czujników i wyświetlaczy.

## 🗂️ Co jest w środku

| Folder | Temat |
|---|---|
| `Lab1` | Wejścia i wyjścia cyfrowe: miganie diodami, „biegnący punkt”, przycisk mono- i bistabilny, eliminacja drgań styków; **schemat układu w KiCad** |
| `Lab2` | **Wyświetlacz siedmiosegmentowy** z multipleksowaniem czterech cyfr, buzzer, minutnik z alarmem |
| `Lab3` | **Timery i przerwania**: Timer1 w trybie CTC, przerwanie zewnętrzne INT0, odświeżanie wyświetlacza w przerwaniu, **enkoder obrotowy** z licznikiem −100…100 |
| `Lab4` | **LCD przez I2C** i przetwornik **ADC**: zegar z budzikiem, woltomierz, włącznik zmierzchowy z histerezą |
| `Lab5` | Komunikacja **USART**: własne funkcje nadawania i odbioru, wykresy pomiarów w Kreślarce, sterowanie układem komendami z terminala |
| `Lab6` | Framework Arduino: termometr **DS18B20** (1-Wire) z wyświetlaniem na LCD, dioda RGB sterowana PWM |
| `Lab7` | Odbiornik **podczerwieni** i **termostat sterowany pilotem** z trybem czuwania i czujnikiem światła |
| `labDodatkowy` | Grafika na wyświetlaczu **OLED** SH1106: bitmapy, tekst i animacja — kartka wielkanocna |
| `Lab_zaliczenie` | Laboratorium zaliczeniowe: wielozadaniowość bez blokujących opóźnień, zegar na przerwaniach, **system pomiarowy sterowany komendami UART** |
| `LCD_I2C` | Biblioteka do obsługi I2C, ekspandera PCF8574 i wyświetlacza HD44780 (z materiałów do zajęć) |

## 🛠️ Technologie

C (AVR-libc) · Arduino · ATmega328P · I2C · USART · ADC · KiCad

## 🚀 Uruchomienie

1. Otwórz wybrany plik `.ino` w Arduino IDE (pliki `.cpp` i `.h` z tego samego folderu dołączą się automatycznie).
2. Wybierz płytkę **Arduino Nano** i port, a następnie wgraj program.

Lab6, Lab7 i lab dodatkowy wymagają bibliotek z menedżera bibliotek Arduino IDE:

```
OneWire · DallasTemperature · LiquidCrystal_I2C · IRremote · Adafruit SH110X · Adafruit GFX
```

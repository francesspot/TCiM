🇵🇱 [Polski](README.md) | 🇬🇧 **English**

# ⚡ Digital and Microprocessor Technology — labs

![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-00878F?logo=arduino&logoColor=white) ![AVR](https://img.shields.io/badge/AVR_ATmega328P-E51B23) ![KiCad](https://img.shields.io/badge/KiCad-314CB0?logo=kicad&logoColor=white)

> 🎓 Lab assignments from the **Digital and Microprocessor Technology** course (Wrocław University of Science and Technology)

Programs for the AVR ATmega328P microcontroller (Arduino Nano) — from blinking an LED to measurement and control circuits. I wrote most tasks in C directly on the microcontroller registers, without the Arduino layer: ports, timers, interrupts, the ADC and the USART are configured bit by bit. Only the last labs use the Arduino framework and ready-made libraries for sensors and displays.

## 🗂️ What's inside

| Folder | Topic |
|---|---|
| `Lab1` | Digital I/O: blinking LEDs, running light, monostable and bistable button, switch debouncing; **KiCad circuit schematic** |
| `Lab2` | **Seven-segment display** with four-digit multiplexing, buzzer, countdown timer with an alarm |
| `Lab3` | **Timers and interrupts**: Timer1 in CTC mode, INT0 external interrupt, interrupt-driven display refresh, **rotary encoder** with a −100…100 counter |
| `Lab4` | **I2C LCD** and the **ADC**: alarm clock, voltmeter, twilight switch with hysteresis |
| `Lab5` | **USART** communication: custom transmit/receive functions, plotting readings in the Serial Plotter, controlling the circuit with terminal commands |
| `Lab6` | Arduino framework: **DS18B20** (1-Wire) thermometer shown on an LCD, RGB LED driven by PWM |
| `Lab7` | **Infrared** receiver and a **remote-controlled thermostat** with standby mode and a light sensor |
| `labDodatkowy` | Graphics on an SH1106 **OLED** display: bitmaps, text and animation — an Easter card |
| `Lab_zaliczenie` | Final assessment lab: multitasking without blocking delays, interrupt-driven clock, **measurement system controlled by UART commands** |
| `LCD_I2C` | I2C, PCF8574 expander and HD44780 display library (from the course materials) |

## 🛠️ Tech stack

C (AVR-libc) · Arduino · ATmega328P · I2C · USART · ADC · KiCad

## 🚀 Running

1. Open the chosen `.ino` file in the Arduino IDE (`.cpp` and `.h` files from the same folder are included automatically).
2. Select the **Arduino Nano** board and port, then upload.

Lab6, Lab7 and the extra lab need these libraries from the Arduino IDE Library Manager:

```
OneWire · DallasTemperature · LiquidCrystal_I2C · IRremote · Adafruit SH110X · Adafruit GFX
```

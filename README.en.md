# ⚡ Digital and Microprocessor Technology

Laboratory solutions for the **Digital and Microprocessor Technology** course (*Technika Cyfrowa i Mikroprocesorowa*), part of the *Automation Information Systems* programme at Wrocław University of Science and Technology.

> 🇵🇱 Polish version: [README.md](README.md)

## 📌 About

The repository contains programs for the **AVR ATmega328P** microcontroller (Arduino Nano) written during consecutive lab sessions. The course progressed from driving single pins to complete measurement and control circuits.

Most tasks (Lab1–Lab5 and the final assessment lab) are written in **plain C directly on the microcontroller registers**, without the Arduino layer: `DDRx/PORTx/PINx` ports, timers, interrupts, the ADC and the USART are configured bit by bit. The last labs (Lab6, Lab7 and the extra lab) use the Arduino framework with libraries for sensors, an IR remote and displays.

## 🧪 Lab scope

| Folder | Topic | What was implemented |
|---|---|---|
| `Lab1` | Digital inputs and outputs | Blinking LEDs, running light, monostable and bistable button, switch debouncing, controlling an LED bar with four buttons. KiCad circuit schematic (`schemat_lab1`). |
| `Lab2` | Seven-segment display and buzzer | Digit code table, multiplexing four digits, sound signalling, a countdown timer with an alarm. |
| `Lab3` | Timers and interrupts | Timer1 in CTC mode, INT0 external interrupt, display multiplexing in a Timer0 interrupt, rotary encoder handling (INT0 and Pin Change Interrupt) with a −100…100 counter and a minus sign. |
| `Lab4` | I2C LCD and the ADC | Button-controlled counter, a clock with an alarm driven by a Timer1 interrupt, an ADC voltmeter, a twilight switch with hysteresis (potentiometer and photoresistor). |
| `Lab5` | USART serial communication | Custom transmit and receive functions, streaming ADC readings to the Arduino Serial Plotter, controlling the circuit with terminal commands, an age calculator reading a number from the console. |
| `Lab6` | Arduino framework, temperature sensor | Voltage measurement with `analogRead`, a DS18B20 (1-Wire) thermometer shown on an LCD, RGB LED control via PWM, a heating indicator. |
| `Lab7` | IR remote and thermostat | Reading infrared remote codes, a remote-controlled thermostat (setpoint, standby mode), extended with a lock when the light level is too low. |
| `labDodatkowy` | OLED display | Bitmaps and text on an SH1106 OLED display (I2C), an animation of falling graphics – an Easter greeting card. |
| `Lab_zaliczenie` | Final assessment lab | Multitasking without blocking delays, a clock and countdown on interrupts, on-demand ADC measurement, a measurement system controlled by UART commands (interrupt-driven reception, single and cyclic mode, channel selection). |
| `LCD_I2C` | LCD library | I2C bus, PCF8574 expander and HD44780 display driver with a demo program. |

## ✨ Highlights

- **Rotary encoder with a display** (`Lab3/zad5_2`) – the program runs entirely on interrupts: Timer0 refreshes the display and a Pin Change interrupt reads the rotation direction. The main loop is empty.
- **Alarm clock** (`Lab4/Zad2`) – timekeeping with a Timer1 CTC interrupt, alarm set with buttons, output on an I2C LCD.
- **Twilight switch with hysteresis** (`Lab4/Zad4`, `Lab5/Zad2_2`) – compares two ADC channels; the Lab5 version also plots the signals in the Serial Plotter.
- **Remote-controlled thermostat** (`Lab7/Zad4`) – DS18B20 sensor, IR receiver, photoresistor and LCD in one circuit.
- **UART measurement system** (`Lab_zaliczenie/zad5`) – a text command interpreter (`jeden`, `cykliczny-X`, `stop`, `CH-X-Y`) with a receive buffer filled in an interrupt.

## 🛠️ Tech stack

- C/C++ (AVR-libc), Arduino framework, Arduino IDE
- ATmega328P microcontroller (Arduino Nano), 16 MHz
- Peripherals: GPIO, Timer0/Timer1, external and Pin Change interrupts, ADC, USART, I2C
- Components: LEDs and an RGB LED, buttons, buzzer, seven-segment display, HD44780 LCD with a PCF8574 expander, SH1106 OLED, rotary encoder, potentiometer, photoresistor, DS18B20 sensor, infrared receiver
- KiCad – schematic and PCB design

## 🚀 Running

1. Open the chosen `.ino` file in the Arduino IDE. `.cpp` and `.h` files from the same folder are included automatically.
2. Select the **Arduino Nano** board and the right port.
3. Upload the sketch.

Programs from Lab6, Lab7 and the extra lab need libraries from the Arduino IDE Library Manager: `OneWire`, `DallasTemperature`, `LiquidCrystal_I2C`, `IRremote`, `Adafruit SH110X` (with `Adafruit GFX`).

Programs using the USART communicate at 19200 bit/s; task 5 of the final lab uses 9600 bit/s.

## 📁 Notes

The library in the `LCD_I2C` folders (`I2C`, `PCF8574`, `LCDI2C` files) comes from the course materials. Its author is named in the file headers.

## 👤 Author

Franciszek Pora – Wrocław University of Science and Technology, Automation Information Systems

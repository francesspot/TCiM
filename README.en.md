# ⚡ Digital and Microprocessor Technology

Laboratory solutions for the **Digital and Microprocessor Technology** course (*Technika Cyfrowa i Mikroprocesorowa*), part of the *Automation Information Systems* programme at Wrocław University of Science and Technology.

> 🇵🇱 Polish version: [README.md](README.md)

## 📌 About

The repository contains microcontroller programs (Arduino, C/C++) driving LEDs and buttons, a KiCad schematic of the lab circuit, and solutions to tasks from subsequent lab sessions.

## 🧪 Contents

### Lab1 – digital I/O (Arduino + KiCad)

| Sketch | Description |
|---|---|
| `miganie_diodami` | LED control – blinking with a set period |
| `walking_point` | "Running light" effect on an LED bar |
| `przycisk_mono` | Monostable button – the LED is on only while the button is held |
| `przycisk_bi` | Bistable button – each press toggles the LED state |
| `dwa_przyciski` | Two buttons controlling outputs |
| `cztery_przyciski` | Four buttons controlling outputs |

`schemat_lab1` – KiCad project (schematic and PCB) of the lab circuit, with a PNG export.

### Lab2

Solutions to tasks 3.2, 4.1 and 4.2 from the lab instructions.

## 🛠️ Tech stack

- Arduino (C/C++), Arduino IDE
- KiCad – schematics and PCB design

## 🚀 Running

1. Open the chosen `.ino` file in the Arduino IDE.
2. Select the board and port under *Tools*.
3. Upload the sketch.

The schematic opens in KiCad via `schemat_lab1.kicad_pro`.

## 👤 Author

Franciszek Pora – Wrocław University of Science and Technology, Automation Information Systems

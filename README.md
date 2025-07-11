# MCU-Programming-UART-Controlled-Smart-Curtain-System-AVR-H-Bridge-
This project implements a **motorized curtain controller** that responds to **serial commands** via UART to open or close a curtain using an **H-Bridge motor driver**. The system also tracks curtain states to avoid redundant motor activation.

---

## Project Overview

- **Microcontroller**: AVR (e.g., ATmega2560)
- **Clock Frequency**: 8 MHz
- **Input**: UART terminal
- **Output**: Motor driver signals (H-Bridge) to control curtain movement
- **Motor Driver**: L293D or similar dual-channel H-Bridge
- **Features**:
  - Command-based curtain control (`open curtain`, `close curtain`)
  - Interrupt-driven UART buffer
  - Simple state-checks to prevent repeated motor commands

---

## UART Command Set

| Command           | Action                |
|-------------------|------------------------|
| `open curtain`    | Rotates the motor in a clockwise manner      |
| `close curtain`   | Rotates the motor anticlockwise              |
| *Any other input* | `"Unknown command"`                          |

---

## How It Works

1. **UART Initialization**:
   - Baud rate: 9600 (double-speed mode enabled)
   - RX complete interrupt triggers command buffering

2. **Motor Control**:
   - Uses two digital pins (PB5 and PB7) to drive the motor via H-Bridge
   - `open_curtain()` and `close_curtain()` functions:
     - Set H-Bridge direction
     - Activate motor for 50 ms
     - Update curtain state flags (`curtain_is_open`, `curtain_is_closed`)

3. **Command Parsing**:
   - If command contains `"open curtain"` and curtain isn’t already open → open it
   - If command contains `"close curtain"` and curtain isn’t already closed → close it
   - Otherwise → `"Unknown command"`

---

## Circuit Setup in Proteus
<img width="618" height="656" alt="image" src="https://github.com/user-attachments/assets/af380ed5-fdcf-46d9-a5bc-140b17269b4e" />

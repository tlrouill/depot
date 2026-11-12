https://github.com/tlrouill/depot

# TTK4155 – Ping-pong game (group XX, H26)

Distributed embedded ping-pong game built in TTK4155 at NTNU.

- **Node 1** – ATmega162 on breadboard + User-I/O board: reads the joystick (through the external ADC), the I/O-board buttons, drives the OLED menu, and talks to node 2 over CAN.
- **Node 2** – Arduino Due (ATSAM3X8E) + motor shield: drives the motor (position controller), the servo and the solenoid, and detects goals with the IR beam.

## Repository layout

| Folder | Content |
|---|---|
| `Node1/` | Node 1 source code (AVR, `avr-gcc`, `Makefile`) |
| `node2-main/` | Node 2 source code (ARM Cortex-M3, `arm-none-eabi-gcc`, `Makefile`) |

### Node 1 (ATmega162)

| File | Role |
|---|---|
| `main.c` | Initialisation, main loop: reads joystick + buttons, sends them over CAN, receives goals, runs the OLED menu |
| `uart_driver.*` | UART driver, `printf` redirected to the PC terminal (9600 baud) |
| `external_memory_driver.*`, `sram_test.*` | External memory interface (XMEM) and SRAM test (test program provided by the course) |
| `adc_driver.*` | External ADC (MAX156): clock from Timer 1, joystick/slider conversion to -100..100, joystick direction |
| `spi_driver.*` | SPI master with three slaves (CAN controller, OLED, I/O board) |
| `oled_driver.*`, `fonts.h` | OLED driver (fonts stored in PROGMEM, `fonts.h` provided by the course) |
| `io_interface_driver.*` | I/O board over SPI: buttons, joystick button, touchpad, slider, LEDs |
| `menu.*` | Non-blocking OLED menu: shows the number of goals, "Reset partie" restarts the game |
| `MCP2515_driver.*` | MCP2515 SPI instructions (read, write, RTS, read status, bit modify, reset) |
| `can_driver.*` | CAN driver on top of the MCP2515 (125 kbit/s, normal mode) |
| `interrupts.*` | INT0 interrupt from the MCP2515 |

### Node 2 (ATSAM3X8E)

| File | Role |
|---|---|
| `main.c` | Initialisation, 10 ms timer callback (jog set-point + controller + solenoid), main loop (CAN messages, goals) |
| `pwm.*` | PWM module driver, channel 0 = motor (PB12), channel 1 = servo (PB13), 1 µs resolution |
| `servo.*` | Servo driver, pulse always limited to 0.9–2.1 ms |
| `tc.*` | Timer/Counter TC0: periodic 10 ms interrupt, `tc_millis()`, `tc_delay_ms()` |
| `encoder.*` | Motor encoder with the TC2 quadrature decoder (PC25/PC26) |
| `motor.*` | Motor driver (shield DRV8871): PWM on PB12, direction on PC23 |
| `position.*` | Calibration (drives to both end stops) and carriage position in encoder ticks |
| `controller.*` | Closed-loop PI position controller (exercise 3.8.4), called every 10 ms |
| `adc.*`, `goal.*` | Internal ADC (IR photodiode), digital filter, goal detection and counting |
| `solenoid.*` | Solenoid pulse on the joystick button (PC12, active low through PNP + relay) |
| `joystick.*` | Decoding of the joystick CAN message |
| `can.*`, `uart.*`, `sam/` | Provided by the course (CAN, UART, CMSIS headers, startup code, linker script) |

## Code not written by the group

- Provided by the course: `node2-main/can.*`, `uart.*`, `time.*`, `sam/`, `Node1/sram_test.c` (SRAM test program), `Node1/fonts.h`.
- AI-assisted code: most of the initialisation functions (`*_init`), as well as various other functions, were written with the help of an AI tool. They are marked with a comment in the files.
- The position controller (PI, `controller.*`) was also implemented with the help of an AI tool.
- This README was written by an AI tool.

# Real-Time Patient Assistance and Caretaker Notification System (ARM Cortex-M4)

A standalone embedded system that lets elderly or bedridden patients signal their needs to a caretaker with a single push button. It's built on the TI TM4C123GH6PM (ARM Cortex-M4) in Embedded C with Keil uVision, and tested on real hardware.

<p align="center">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.23%20A.jpeg" width="32%" alt="Hardware test photo">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.23%20AM.jpeg" width="32%" alt="Hardware test photo">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.24%20AM.jpeg" width="32%" alt="Hardware test photo">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.25%20.jpeg" width="32%" alt="Hardware test photo">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.25%20AM.jpeg" width="32%" alt="Hardware test photo">
  <img src="images/WhatsApp%20Image%202026-11-22%20at%2012.47.26%20.jpeg" width="32%" alt="Hardware test photo">
</p>

*Photos from hardware testing on the TM4C123 LaunchPad trainer kit: system ready, water and food requests, emergency alert and emergency attended.*

## Features

- **Multi-press requests on one button:** 1 press = water, 2 presses = food, 3 or more = medicine
- **16x2 LCD** shows the request and the system status
- **Buzzer alert** for every new request
- **Reminder alerts.** The buzzer repeats every 5 seconds until the caretaker acknowledges the request.
- **Emergency mode.** The LED turns on, the LCD shows `EMERGENCY!!! / CALL NURSE`, and the buzzer beeps continuously until the emergency is attended.
- **Caretaker acknowledgement**, which returns the system to standby

## How it works

1. On power-up, the LCD shows `PATIENT CARE / SYSTEM READY`.
2. **Patient request (SW1).** The controller counts presses until 4 seconds pass with no new press, then shows `PATIENT NEED` with WATER, FOOD or MEDICINE and sounds the buzzer.
3. **Acknowledge (SW2).** The caretaker presses SW2, the LCD shows `REQUEST ATTENDED`, and the system returns to standby. Until then, the buzzer repeats every 5 seconds.
4. **Emergency (SW2 in standby).** The LED and continuous buzzer turn on. Pressing SW1 stops the alarm and shows `EMERGENCY ATTENDED`.

## Hardware and pin connections

| Function | TM4C123GH6PM pin |
|---|---|
| SW1: patient request / stop emergency | PF4 (onboard switch, internal pull-up) |
| SW2: emergency / caretaker acknowledge | PF0 (onboard switch, unlocked, internal pull-up) |
| Status LED | PF2 |
| Buzzer | PB3 |
| LCD shift register: serial data / clock | PF2 / PF3 |
| LCD shift register: latch | PE5 |

The 16x2 LCD runs in 4-bit mode and is driven through an 8-bit shift register on the ARM Cortex-M4 trainer kit, which saves GPIO pins. The LCD nibble bits are remapped in software to match the board wiring. Delays use the SysTick timer at the default 16 MHz clock.

## Software

| Function | Purpose |
|---|---|
| `PortF_Init`, `PortB_Init`, `PortE_Init` | GPIO setup for the switches, LED, buzzer and LCD shift register |
| `LCD_init`, `LCD_command`, `LCD_putc`, `LCD_puts` | 4-bit LCD driver over the shift register |
| `shift_out1` | Bit-bangs one byte into the shift register and latches it |
| `Count_Presses` | Debounced multi-press counter with a 4 s timeout |
| `buzzer_short` | 500 ms buzzer beep |
| `delayMs` | Millisecond delay using SysTick |

## Build and run

1. Create a Keil uVision project for the **TM4C123GH6PM** and add `src/main.c`. The project also needs the `tm4c123gh6pm.h` header from the TivaWare / Keil device pack.
2. Build it and flash it to the TM4C123 LaunchPad.
3. Press SW1 once, twice or three times to send a request, SW2 to acknowledge, and SW2 from standby to trigger an emergency.

## Notes and future work

- PF2 drives both the LED and the shift register's serial data line, so the LED flickers while the LCD updates.
- Possible next steps: GSM or Wi-Fi notifications to the caretaker's phone, more request types, and health sensors such as pulse and temperature.

Team project for the Embedded Systems course at Amrita Vishwa Vidyapeetham, Amritapuri (May 2026).

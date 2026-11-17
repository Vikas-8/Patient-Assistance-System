# Real-Time Patient Assistance and Caretaker Notification System

## Project Overview

This project focuses on developing a real-time patient assistance system using the ARM Cortex-M4 based TM4C123GH6PM microcontroller.

The system enables patients, particularly elderly and physically weak individuals, to communicate their basic needs to caretakers through a simple push-button interface. It identifies patient requests based on the number of button presses and provides visual and audible notifications.

An emergency switch is also integrated to generate priority alerts for immediate caretaker attention.

## Key Features

- Single-button multi-press detection.
- Water, food and medicine request identification.
- 16×2 LCD display for request information.
- Buzzer-based caretaker notifications.
- Emergency switch with priority alert.
- LED-based emergency indication.
- Caretaker acknowledgement mechanism.
- Automatic reminder alerts for unattended requests.

## Hardware Components

- TM4C123GH6PM ARM Cortex-M4 microcontroller.
- 16×2 LCD display (JHD162A).
- 74HC595 shift register.
- Push buttons and emergency switch.
- Piezo buzzer.
- LED indicators.

## Working Principle

The system continuously monitors the patient request button.

| Button Presses | Patient Request |
|---|---|
| 1 Press | Water |
| 2 Presses | Food |
| 3 or more Presses | Medicine |

The selected request is displayed on the LCD, and the buzzer notifies the caretaker.

If the request is not acknowledged within the configured time, the system generates reminder alerts.

The emergency switch activates a priority alert using continuous buzzer and LED indications until the emergency is attended and cleared.

## Software and Technologies

- Embedded C
- ARM Cortex-M4
- TM4C123GH6PM
- Keil µVision
- GPIO Interfacing
- LCD Interfacing
- Shift Register Communication
- SysTick Timer

## Testing and Results

The system was evaluated under different operating conditions, including:

- Standby operation.
- Water request detection.
- Food request detection.
- Medicine request detection.
- Emergency alert activation.
- Caretaker acknowledgement.

## Applications

- Patient assistance systems.
- Elderly care environments.
- Hospital patient notification systems.
- Assisted living facilities.

## Future Scope

- Wireless notification to caretakers.
- Mobile application integration.
- IoT-based remote monitoring.
- Additional patient monitoring sensors.
- Improved power management.

## Conclusion

The project demonstrates an embedded patient assistance solution using ARM Cortex-M4, providing a simple interface for patient requests and emergency notifications to caretakers.

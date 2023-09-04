# Vibrator with PWM Motor Controller

## Introduction

This project aims to control the vibrator head of the European Magic Wand (EMW) from Denmark using a DIY motor controller. Essentially, this PWM motor controller can be used to precisely control DC motors. The foundation of this project is the ATTiny85 microcontroller, which enables precise motor speed control through Pulse Width Modulation (PWM). This project requires electronics, soldering, and programming skills. The primary goal is to replace the original EMW's 220V input voltage with a lower input voltage, such as from a power tool battery.

**Note:** The Arduino Sketch is available under `/software`, and the Fritzing files are under `/hardware`.

## Features

- Replication of a vibrator with the original head of the European Magic Wand (EMW).
- Precise control of vibration speed through Pulse Width Modulation (PWM).
- Support for various input voltages through the use of suitable voltage regulators and transistors.
- Utilization of the ATtiny85 microcontroller for motor control.
- External controller board with three buttons and LEDs for user interaction.
- Sleep mode for energy conservation when turned off.

## Requirements

- The two PCB boards from the `/hardware` directory.
- ATtiny85 microcontroller.
- DC Motor (775 series DC motor)
- Fixed voltage regulator (e.g., 78L05) for logic supply.
- Transistor (e.g., TIP120) for motor supply.
- Various resistors, diodes, and capacitors.
- Soldering skills.
- Programming skills for flashing the ATtiny85 firmware.

## Usage

1. Understand the schematics and adjust resistors, voltage regulators, capacitors, and diodes according to requirements.

2. Solder the components onto the PCB boards.

3. Connect the external controller board to the motor controller using cables.

4. Program the ATtiny85 microcontroller with the provided firmware from the `/software` directory using the Arduino IDE and an ISP. It is recommended not to solder the ATTiny directly but to use a DIP8 IC socket.

5. Connect the motor to the motor output of the motor controller and the power source to the voltage input.

6. Use the buttons on the controller board to activate motor control and adjust the PWM phase.

## Integration with the European Magic Wand

This motor controller project is part of a larger project to replicate the vibrator head of the European Magic Wand (EMW) from Denmark. The necessary STL files for the vibrator body can be found on [Thingiverse](https://www.thingiverse.com/thing:5776940).

## License

This project is licensed under the CC0 1.0 Universal License. Further information can be found in the `LICENSE` file.
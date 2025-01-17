# DSFUM Final Project by Cristea Olaru Stefan

## Introduction

I wanted to create a system that allows me to remotely control my laptop. The idea involves integrating an Arduino with an infrared (IR) receptor to communicate with my laptop. Using a program on my phone, I can send IR signals that are captured and interpreted to execute specific commands on my laptop.

## How It Works

The system consists of two primary components:

1. **Arduino and IR receptor:** The Arduino receives IR signals transmitted by my phone. These signals are encoded patterns corresponding to specific commands. I had to use a library called [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
2. **Laptop application:** A program running on my laptop receives and decodes these signals, mapping them to predefined shell commands.

When the Arduino detects an IR signal, it forwards the encoded signal to the laptop via a serial connection. The laptop program reconstructs the signal, identifies the corresponding command, and executes it.

### Workflow
1. IR signals are sent from the phone program.
2. The IR receptor attached to the Arduino detects these signals.
3. The Arduino processes the signal and sends it to the laptop over USB.
4. The laptop program decodes the signal and performs the associated shell command.

## The Physical Part

The hardware setup is straightforward, utilizing an IR receptor and a breadboard:

- **IR receptor:** It has three pins:
  - **Left pin (Output):** Connected to one of the Arduino's digital input pins. ( In my case the third pin )
  - **Middle pin (Ground):** Connected to the Arduino's GND.
  - **Right pin (Power supply):** Connected to the Arduino's 5V pin.

- **Breadboard:** The breadboard simplifies the wiring process, allowing the components to be neatly arranged and securely connected.

To assemble the circuit:
1. Place the IR receptor on the breadboard.
2. Connect the output pin to a digital input pin on the Arduino.
3. Attach the ground pin to the Arduino’s GND pin.
4. Connect the power pin to the Arduino’s 5V pin.

Below is an illustration of the wiring setup:

### Diagram
- **IR Receptor**
  - Left pin — Output (Digital Pin X)
  - Middle pin — Ground
  - Right pin — Power Supply (5V)

![Board Diagram](image.png)

## Software Implementation

### Arduino Code
The Arduino code is responsible for:
- Reading IR signals from the receptor.
- Sending these signals to the laptop via a serial connection.

Here’s an overview of the code logic:
1. Initialize the IR receptor library and configure the input pin.
2. Continuously listen for IR signals.
3. Send detected signals to the laptop in a format it can process.

### Laptop Application
The laptop application handles:
- Receiving and decoding signals sent by the Arduino.
- Mapping the decoded signals to specific shell commands.
- Executing the commands in the shell environment.

The application could be written in Python using libraries like `pyserial` to read data from the Arduino and `subprocess` to execute shell commands.

#### Code Features:
- A configuration file to define the mapping between IR signals and commands.
- Error handling to ensure invalid or unrecognized signals do not crash the system.

## Conclusion

This project demonstrates how simple components like an IR receptor, Arduino, and some programming can combine to create a functional and useful system. It can be expanded further to include more commands, better signal processing, or even a graphical interface for easier configuration. The integration of hardware and software highlights the flexibility and power of microcontrollers in everyday applications.

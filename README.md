# SmartKey Studio: An Interactive LED Piano Learning System Powered by Arduino Uno

This project is an end-to-end, hardware-software integrated piano tutor that visually guides users through songs with real-time LED feedback directly on the keyboard. It showcases a full product development cycle—from initial concept and circuit design to firmware programming and system integration—to create a practical and engaging tool for learning music.

## Core Functionality

The system processes live MIDI data from a digital piano and maps each note to a corresponding LED on a custom-built light strip placed above the keys. When a song is played, the LEDs light up to show the user which keys to press and when, providing an intuitive "follow the lights" learning experience.

## Hardware Integration and Design

The electronic circuit was designed and assembled on a breadboard, centered around an Arduino Uno microcontroller.

*   **LED Strip**: A WS2812B addressable LED strip is powered by an external 5V/4A source.
*   **Power Regulation**: A parallel capacitor is used to regulate the power supply and prevent flickering.
*   **Signal Integrity**: A 330-ohm resistor is placed on the data line to protect the LED strip from voltage spikes and ensure reliable communication.

## Custom API and Firmware

The core of the project is a modular Arduino script (C++) that functions as a customizable API. It is engineered to be easily edited, allowing a user to load new MIDI song files or modify lighting preferences. This design transforms the firmware from a static program into a flexible engine for personalized learning.

## Software-Hardware Interfacing

To bridge the software and hardware components, the system utilizes the **Hairless MIDI Serial Bridge**. This tool converts MIDI signals from the piano into serial data that the Arduino Uno can read and process, enabling seamless, real-time communication between the musical input and the visual LED output.
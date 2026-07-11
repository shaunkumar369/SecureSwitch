## SecureSwitch
An integrated IoT system combining non-contact hardware sensing with interactive real-time applications.
SecureSwitch leverages the Time-of-Flight(ToF) principle via an ultrasonic sensor to create a touchless user interface secured by a distance based gesture password menu system.'

## Project Overview

The core objective of SecureSwitch is to explore Human-Computer Interaction (HCI) using non-contact sensors.
The system operates in four distinct software modes, navigated seamlessly through precise distance gestures:

1. **Secure Lock:** The default state requiring specific hand-distance sequences to unlock the device.
2. **Distance Radar:** A precision utility that measures and visualizes proximity in real-time.
3. **Reaction Timer:** An F1-inspired competitive reflex tester featuring live performance feedback.
4. **Flappy Plane / Flappy Bird:** A physics-based side-scroller mini-game where flight height is dynamically controlled by hand elevation.

## Hardware & Architecture

### Components Used
* **Microcontroller:** Arduino Uno (polling and low-latency data acquisition)
* **Sensor:** HC-SR04 Ultrasonic Sensor (sound pulse emission and echo timing)
* **Display Output:** Supports built-in 128x64 I2C OLED display (SSD1306) or high-definition external serial visualization hubs.
* **Interconnects:** USB Data Cable, Breadboard, and Jumper Wires.

### Architecture Workflow
* **Data Collection:** The HC-SR04 measures hand distance via the Time-of-Flight principle. The Arduino micro-controller processes the raw sensor pulse timings using the optimized `NewPing` library.
* **Menu Control & Sticky Logic:** Security validation is achieved via a dedicated success counter mechanism requiring the user to hold their hand steady at target distance thresholds for consecutive clock cycles.
* **Application Physics:** Real-time data mapping (`map()` and `constrain()`) translates physical movement into game physics or display indicators.

## Repository Structure

* `SecureSwitch.ino` - The complete Arduino implementation containing setup routines, sensor integration loop, multi-app state machine logic, and screen rendering scripts.
* `SecureSwitch.pdf` - Complete documentation containing project abstract, hardware specifications, and system architecture breakdowns.

##  Installation & Setup

1. **Hardware Assembly:**
   * Connect the HC-SR04 Ultrasonic Sensor to the Arduino Uno:
     * **VCC** ➡️ 5V
     * **GND** ➡️ GND
     * **Trig** ➡️ Pin 9
     * **Echo** ➡️ Pin 10
   * Connect the SSD1306 OLED display via I2C (`SDA` / `SCL` pins).

2. **Software Dependencies:**
   Ensure the following libraries are installed in your Arduino IDE:
   * `Wire.h`
   * `Adafruit_GFX.h`
   * `Adafruit_SSD1306.h`
   * `NewPing.h`

3. **Flashing the Controller:**
   * Open `SecureSwitch.ino` in your Arduino IDE.
   * Verify and upload the sketch to your Arduino Uno board.

## Project Contributors
Developed by a team of Bachelor of Computer Applications (BCA) students at **Seshadripuram Degree College**:
* Shaun Kumar
* Liran Kaverappa K J
* Thejus Kumar N
* Mayur K
* Manoj M
* Manjunath K

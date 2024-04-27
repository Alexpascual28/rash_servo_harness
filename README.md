<p align="center">
  <img src="https://www.svgrepo.com/show/296321/industrial-robot-robot.svg" width="100" alt="project-logo">
</p>
<p align="center">
    <h1 align="center">RASH Servo Harness</h1>
</p>
<p align="center">
    <em><code> RASH (Robot Arm Servo Harness) – Arduino firmware for a custom servo harness that attaches to the EVA robot arm end-efector. The firmware interfaces between the two GPIO pins of the arm and an array of breakout servo pins. Using Arduino IDE and C++. </code></em>
</p>

<br><!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary><br>

- [ Overview](#-overview)
- [ File Details](#file-details)
- [ Repository Structure](#-repository-structure)
- [ Modules](#-modules)
- [ Getting Started](#-getting-started)
  - [ Requirements](#requirements)
  - [ Installation](#-installation)
- [ Code](#code)
- [ Contributing](#-contributing)

</details>
<hr>

##  Overview

<code>The RASH (Robot Arm Servo Harness) firmware is designed for a custom servo harness that attaches to the EVA robot arm end-effector. This firmware facilitates the communication between the two GPIO pins of the arm and an array of breakout servo pins. The project is developed using Arduino IDE and C++.</code>

---

##  File Details:

`Arduino_Servo_Harness_Firmware/`: This directory holds the firmware for the standard servo control operation of the RASH system. The firmware allows for basic open and close operations based on button inputs and potentiometer readings.

* `Arduino_Servo_Harness_Firmware.ino`: The main Arduino sketch file which includes the setup and loop functions, servo control logic, calibration routines, and serial communication setup. This file outlines the basic operation for interacting with the GPIO pins and controlling the servo positions.

`Arduino_Servo_Harness_Firmware_Continuous/`: This directory contains a version of the firmware that supports continuous operation, meaning the servos can be operated in a mode where they rotate continuously based on the inputs, rather than moving to specific positions.

* `Arduino_Servo_Harness_Firmware_Continuous.ino`: Similar to the standard firmware but includes adjustments or additional functionalities to allow continuous rotation and operation of the servos, utilizing the interrupts for button presses to change the gripper state dynamically.

`README.md`: This markdown file provides an overview of the project, installation instructions, hardware and software requirements, configuration details, and usage instructions. It serves as the main documentation source to help users set up and understand how to operate the servo harness.

---

##  Repository Structure

```sh
└── rash_servo_harness/
    ├── Arduino_Servo_Harness_Firmware
    │   └── Arduino_Servo_Harness_Firmware.ino
    ├── Arduino_Servo_Harness_Firmware_Continuous
    │   └── Arduino_Servo_Harness_Firmware_Continuous.ino
    └── README.md
```

---

##  Modules

<details closed><summary>Arduino_Servo_Harness_Firmware_Continuous</summary>

| File                                                                                                                                                                                                         | Summary                         |
| ---                                                                                                                                                                                                          | ---                             |
| [Arduino_Servo_Harness_Firmware_Continuous.ino](https://github.com/Alexpascual28/rash_servo_harness.git/blob/master/Arduino_Servo_Harness_Firmware_Continuous/Arduino_Servo_Harness_Firmware_Continuous.ino) | <code> </code> |

</details>

<details closed><summary>Arduino_Servo_Harness_Firmware</summary>

| File                                                                                                                                                                        | Summary                         |
| ---                                                                                                                                                                         | ---                             |
| [Arduino_Servo_Harness_Firmware.ino](https://github.com/Alexpascual28/rash_servo_harness.git/blob/master/Arduino_Servo_Harness_Firmware/Arduino_Servo_Harness_Firmware.ino) | <code> </code> |

</details>

---
##  Getting Started

###  Requirements

**Hardware**

* Arduino Nano
* Servo motors (6x)
* Potentiometers (2x)
* Buttons (2x)
* Indicator LED

**Software**

* Arduino IDE
* Servo library

###  Installation

1. Set up your Arduino environment:

* Download and install the Arduino IDE from the Arduino website.
* Connect your Arduino board to your computer.

2. Install the Servo library:

* Open the Arduino IDE, go to `Sketch` > `Include Library` > `Manage Libraries...`
* Search for `Servo` and install the library.

3. Load the firmware:

* Download the firmware files from this repository.
* Open the `.ino` file with the Arduino IDE.
* Select your Arduino board type and port under `Tools`.

4. Compile and upload the firmware:

* Click on the `Upload` button in the Arduino IDE to compile and upload the firmware to your Arduino board.

##  Code

### Configuration

* Pin Configuration:

> ```cpp
> #define INDICATOR 3
> #define BUTTON1 A3
> #define BUTTON2 A2
> #define POT1 A7
> #define POT2 A6
> #define SERVO1 12
> #define SERVO2 11
> #define SERVO3 10
> #define SERVO4 9
> #define SERVO5 8
> #define SERVO6 7
> ```

* **Initial Setup in `setup()`:**

* Configure the pin modes, attach servos, and initialize serial communication.

> ```cpp
> void setup() {
>   pinMode(INDICATOR, OUTPUT);
>   for (int i = 0; i < 6; ++i) {
>     servos[i].attach(12-i);
>   }
>   Serial.begin(115200);
> }
> ```

###  Functionality

**Main Loop**

The `loop()` function updates sensor readings, checks for button presses to open/close the gripper, and updates the heartbeat LED.

**Calibration**

Use potentiometers to calibrate the open and closed positions of the servos dynamically. Example:

> ```cpp
> void calibrate(int servoId, int pot, bool targetState) {
>   int reading = analogRead(pot);
>   int position = map(reading, 0, 1023, 0, 180);
>   if (targetState == OPEN) openPositions[servoId-1] = position;
>   else closedPositions[servoId-1] = position;
> }
> ```

**Gripper Control**

Control the gripper state with functions `openGripper()` and `closeGripper()`, which iterate through the servos to set their positions based on the calibrated values.

---

##  Troubleshooting

* **LED Indicator not working**: Check if the LED is properly connected to pin INDICATOR and that it is configured as OUTPUT.
* **Servos not responding**: Ensure that each servo is properly connected and powered. Check that the servo pins are correctly defined in the code.
* **Serial Monitor outputs erratic values**: Verify the baud rate settings in both the Arduino code and the Serial Monitor are set to 115200.

For more detailed information on modifying or extending the firmware functionalities, please refer to the inline comments within the `.ino` files provided in this repository.

---

##  Contributing

Contributions are welcome! Here are several ways you can contribute:

- **[Report Issues](https://github.com/Alexpascual28/rash_servo_harness.git/issues)**: Submit bugs found or log feature requests for the `rash_servo_harness` project.
- **[Submit Pull Requests](https://github.com/Alexpascual28/rash_servo_harness.git/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.
- **[Join the Discussions](https://github.com/Alexpascual28/rash_servo_harness.git/discussions)**: Share your insights, provide feedback, or ask questions.

<details closed>
<summary>Contributing Guidelines</summary>

1. **Fork the Repository**: Start by forking the project repository to your github account.
2. **Clone Locally**: Clone the forked repository to your local machine using a git client.
   ```sh
   git clone https://github.com/Alexpascual28/rash_servo_harness.git
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to github**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.
8. **Review**: Once your PR is reviewed and approved, it will be merged into the main branch. Congratulations on your contribution!
</details>

<details closed>
<summary>Contributor Graph</summary>
<br>
<p align="center">
   <a href="https://github.com{/Alexpascual28/rash_servo_harness.git/}graphs/contributors">
      <img src="https://contrib.rocks/image?repo=Alexpascual28/rash_servo_harness.git">
   </a>
</p>
</details>

---

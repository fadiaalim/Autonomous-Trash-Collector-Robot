# Autonomous Trash Collector Robot 🤖♻️

An intelligent mobile robot designed to navigate autonomously, detect obstacles using Ultrasonic sensors, and perform precise pick-and-place operations using a 3-DOF robotic arm controlled by a **Pulse-Width Modulation (PWM)** matrix.

## 🌟 Key Features

* **Autonomous Navigation:** Real-time obstacle detection and movement control.
* **Precise Kinematics:** Robotic arm movements are driven by a custom-calibrated **Pulse Matrix** (150-600 pulses) for high accuracy.
* **I2C Communication:** Uses PCA9685 servo driver to manage multiple servos with minimal wiring.
* **Computer Vision Integration:** Ready to interface with Python/OpenCV for object classification and decision making.

## 🛠️ Hardware Stack

* **Controller:** Arduino Uno
* **Servo Driver:** Adafruit PCA9685 (16-Channel 12-bit PWM)
* **Actuators:** 3x SG90/MG90s Servos (Base, Shoulder, Elbow) + 2x DC Motors
* **Sensors:** HC-SR04 Ultrasonic Sensor
* **Motor Driver:** L298N / L293D

## 💻 Software & Libraries

1. **Python (Computer Vision & Decision Making)**
The Python script captures the camera feed, detects objects, and communicates with the Arduino via Serial.
Task: Detects trash, draws bounding boxes, and sends the character 'P' (Pick) or 'S' (Skip) to the robot.
Communication: Serial Interface @ 9600 Baud.

2. **Arduino (Hardware Execution)**
The firmware handles autonomous driving and the "Pick & Place" sequence.
Logic: Once the Ultrasonic sensor detects an object at 20cm, it waits for the Python "Pick" command.
Matrix Execution: Uses the smoothPulse function to transition between simulation keyframes.



## 📊 Matrix Configuration

The robot operates based on a specific starting and target matrix to ensure smooth and repeatable motions:

| Joint | Home Pulse (Start) | Target Pulse (Pick) | Channel |
| --- | --- | --- | --- |
| **Base** | 520 | 470 | 0 |
| **Shoulder** | 110 | 110 | 1 |
| **Elbow** | 230 | 260 | 2 |

## 🚀 System Logic

1. **Search Phase:** Robot moves forward until the Ultrasonic sensor detects an object within 20cm.
2. **Detection:** Sends a `DETECT` signal via Serial to the Python backend.
3. **Command:** Upon receiving the character `'P'`, the robot executes the `pickAndPlace()` sequence.
4. **Sequence:**
* Moves arm to **Target Matrix**.
* Lifts the arm and rotates **180°** to deposit the item in the rear container.
* Returns to **Home Matrix** and resumes navigation.



---

### How to Use

1. Clone the repository.
2. Connect a 5V external battery to the PCA9685 terminal (Required for servo movement).
3. connect a 12v battery to the driver moduel for wheels.
4. Hardware: Wire the PCA9685 to Arduino (A4/A5) and connect an external 5V battery.
5.Arduino: Upload the code from /Arduino_Code.
Python: Install dependencies (pip install opencv-python pyserial) and run the script.
Operation: The robot will drive forward; once it detects an object, it will pause for Python to confirm the "Pick" command.
Autonomous-Trash-Collector-Robot

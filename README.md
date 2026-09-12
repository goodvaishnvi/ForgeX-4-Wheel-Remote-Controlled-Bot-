# ForgeX-4-Wheel-Remote-Controlled-Bot-
A 4-wheel RC robotic bot controlled by a remote, featuring a 3D-printed arm for lifting and handling lightweight objects.

## ✨ Features

- 4-wheel drive system
- Remote-controlled movement
- ESP32-WROOM based control
- 3D-printed robotic arm
- 4-servo robotic arm control
- Lightweight object lifting and handling
- 16×2 I2C LCD for displaying robot status
- Dual L298N motor driver system
- FlySky i6X transmitter and receiver

## 🛠️ Hardware Components

- ESP32-WROOM
- 2 × L298N Motor Drivers
- 4 × DC Gear Motors
- FlySky i6X Transmitter & Receiver
- 4 × Servo Motors
- 16×2 I2C LCD
- 4-Wheel Chassis
- 3D-Printed Robotic Arm
- Battery / Power Supply

## 💻 Software

- Arduino IDE
- C++ (Arduino Framework)
- ESP32 Board Package
- LiquidCrystal_I2C Library

## ⚙️ Working Principle

The ForgeX bot is controlled wirelessly using a FlySky i6X remote transmitter. The receiver sends control signals to the ESP32-WROOM.

The ESP32 processes the receiver signals and controls two L298N motor drivers to operate the four DC motors.

The additional receiver channels control four servo motors connected to the 3D-printed robotic arm. The arm can be used to lift and handle lightweight objects.

A 16×2 I2C LCD displays the current movement status and motor speed.

## 🔧 Control System

| Receiver Channel | Function |
|---|---|
| CH1 | Steering |
| CH2 | Forward / Reverse |
| CH3 | Arm Base Rotation |
| CH4 | Arm Base Tilt |
| CH5 | Elbow Movement |
| CH6 | Gripper |

## 📂 Project Structure

```text
ForgeX-4-Wheel-RC-Bot/
│
├── README.md
├── src/
│   └── ForgeX.ino
│
├── hardware/
│   ├── circuit-diagram/
│   └── components.md
│
├── 3d-models/
│   ├── robotic-arm/
│   └── chassis/
│
├── images/
│
├── docs/
│
├── .gitignore
└── LICENSE

##👥 Team Members

Vaishnavi Surwase
Sharwani Sawant
Sanskar Malore
Bhoomi Oswal
Prathamesh

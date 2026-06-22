# ESP32-MPU6050-MultiTool

A multifunction embedded systems project built using ESP32, MPU6050 and SSD1306 OLED display.

## Features

- Accelerometer Monitor
- Gyroscope Monitor
- Bubble Level Indicator
- Vibrometer
- Real-Time Vibration Graph
- OLED Menu Navigation

## Hardware Used

- ESP32
- MPU6050
- SSD1306 OLED Display (128x64)
- Push Buttons
- Breadboard and Jumper Wires

## How It Works

The ESP32 reads motion data from the MPU6050 sensor and displays different tools through a menu-driven OLED interface.

### Accelerometer Screen
Displays X, Y and Z acceleration values.

### Gyroscope Screen
Displays angular velocity on all three axes.

### Bubble Level
Maps sensor tilt to a movable bubble on the OLED display.

### Vibrometer
Calculates vibration magnitude and displays:
- Peak vibration
- Average vibration
- Real-time vibration graph

## Project Status

Version 1.0 Completed

## Future Improvements

- Data Logging
- Bluetooth Connectivity
- Calibration Menu
- Improved Graph Scaling

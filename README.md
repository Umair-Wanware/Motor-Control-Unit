# Motor Control Unit

> A modular, RTOS-ready motor control firmware for STM32, designed with a layered architecture, reusable C drivers, and a C++ application framework. The project aims to provide a scalable platform for DC, BLDC, and PMSM motor control with future support for advanced algorithms such as PID, FOC, and real-time diagnostics.

---

## Overview

The **Motor Control Unit** is an embedded firmware project built around the **STM32F103RB** microcontroller using **STM32Cube HAL**, **PlatformIO**, and **FreeRTOS**.

The project follows an industry-style architecture by separating hardware drivers, middleware, and application logic, making it easy to maintain, extend, and reuse across multiple motor control applications.

---

## Features

### Hardware Drivers

- GPIO Driver
- ADC Driver
- UART Driver
- SPI Driver
- I2C Driver
- PWM Driver
- Encoder Driver

### Architecture

- Modular Driver Layer
- Hardware Abstraction Layer (HAL)
- FreeRTOS Ready
- C Drivers
- C++ Application Layer
- Layered Software Design
- Scalable Firmware Architecture

### Communication

- UART Debug Interface
- SPI Communication
- I2C Peripheral Support

---

## Planned Features

### Sensor Drivers

- Current Sensor
- Voltage Sensor
- Temperature Sensor
- Hall Sensor

### Motor Control

- Motor Driver
- Motor State Machine
- PID Speed Controller
- Ramp Generator
- Digital Filters
- Speed Estimation

### Protection

- Over Current Protection
- Over Voltage Protection
- Under Voltage Protection
- Over Temperature Protection
- Motor Stall Detection
- Encoder Fault Detection
- Emergency Stop

### RTOS

- Sensor Task
- Control Task
- Motor Task
- Communication Task
- Diagnostics Task
- Logger Task

### Diagnostics

- CPU Load Monitoring
- Stack Usage
- Heap Usage
- Fault Logger
- Runtime Statistics
- System Health Monitor

### Communication

- STM32 ↔ ESP32 Communication
- Binary Packet Protocol
- CRC Validation
- Telemetry Streaming

### Dashboard

- ESP32 Web Dashboard
- Live Motor Speed
- Current Monitoring
- Voltage Monitoring
- Temperature Monitoring
- Fault Visualization
- Live Graphs
- Configuration Interface

---

# Software Architecture

```
                    +----------------------+
                    |    Application Layer |
                    +----------------------+
                              |
                    +----------------------+
                    |      Middleware      |
                    +----------------------+
                              |
                    +----------------------+
                    |    Device Drivers    |
                    +----------------------+
                              |
                    +----------------------+
                    |     STM32 HAL        |
                    +----------------------+
                              |
                    +----------------------+
                    |      Hardware        |
                    +----------------------+
```

---

# Project Structure

```
Motor-Control-Unit
│
├── include/
│
├── src/
│   └── main.cpp
│
├── lib/
│   ├── gpio/
│   ├── adc/
│   ├── uart/
│   ├── spi/
│   ├── i2c/
│   ├── pwm/
│   ├── encoder/
│   └── FreeRTOS/
│
├── test/
│
├── platformio.ini
│
└── README.md
```

---

# Hardware

### Microcontroller

- STM32F103RB
- ARM Cortex-M3
- 72 MHz
- 128 KB Flash
- 20 KB SRAM

### Development Environment

- PlatformIO
- Visual Studio Code
- STM32Cube HAL
- FreeRTOS

---

# Driver Status

| Driver | Status |
|---------|--------|
| GPIO | ✅ Complete |
| ADC | ✅ Complete |
| UART | ✅ Complete |
| SPI | ✅ Complete |
| I2C | ✅ Complete |
| PWM | ✅ Complete |
| Encoder | ✅ Complete |

---

# Development Roadmap

## Phase 1

- GPIO
- ADC
- UART
- SPI
- I2C
- PWM
- Encoder

✅ Completed

---

## Phase 2

- Current Sensor
- Voltage Sensor
- Temperature Sensor
- Hall Sensor

---

## Phase 3

- PID Controller
- Motor Driver
- Speed Controller
- Ramp Generator

---

## Phase 4

- Fault Manager
- Safety Manager
- Diagnostics
- Logger

---

## Phase 5

- FreeRTOS Integration
- Task Scheduler
- Queue Communication
- Event Groups

---

## Phase 6

- STM32 ↔ ESP32 Communication
- Binary Protocol
- CRC Validation

---

## Phase 7

- ESP32 Web Dashboard
- Live Monitoring
- OTA Updates
- Configuration Interface

---

## Phase 8

- CAN Bus
- USB
- SD Card Logging
- Bootloader
- Firmware Updates

---

# Future Goals

- Field Oriented Control (FOC)
- Space Vector PWM (SVPWM)
- Sensorless BLDC Control
- PMSM Control
- Data Logging
- CANopen Support
- Motor Parameter Auto-Tuning
- Advanced Diagnostics
- Mobile Dashboard
- Cloud Monitoring

---

# Build

```bash
git clone https://github.com/Umair-Wanware/Motor-Control-Unit.git

cd Motor-Control-Unit

pio run
```

---

# License

This project is licensed under the MIT License.

---

# Author

**Umair Wanware**

Embedded Software Developer

---

## Project Status

**Current Version:** **v1.0.0**

**Development Status:** 🟢 Active Development

This project is under continuous development with the goal of becoming a professional, industry-grade motor control firmware platform for STM32 microcontrollers.


# LIMPID C code structure

<p align="center">
  <img src="/esp-code/images/diagram_system.png" alt="Limpid Project" width="300">
</p>

Limpid Water Statistics is an ESP-32 microcontroller-based project aimed at collecting and processing various water quality parameters. The system is structured into several main tasks and components, each responsible for handling specific functionalities and sensor data.

## Overview

The project structure is divided into tasks, folders, `.c` files, and `.h` files, with each element playing a specific role in the system. Below is an overview of the project's architecture:

### Tasks

- **task1**
  - **LMPD_blt**: Responsible for Bluetooth communication.
  - **LMPD_ch**: Main controller handling sensor data acquisition and communication.

- **task2**
  - **LMPD_wpp**: Manages water pumping process.

- **task3**
  - Bluetooth offline task.

### Hardware Components

- **ESP-32 MCU**: The central microcontroller unit managing the entire system.

### Main Folders

- **LMPD_ch**: Encompasses core components and functionalities related to sensor data handling.
- **LMPD_sen**: Contains sensor-specific files and drivers.
- **LMPD_rom**: microSD related functionalities.
- **LMPD_wpp**: Water Type processing;.
- **LMPD_blt**: Bluetooth drivers.

### Sensors

- **SEN_ds**: Manages temperature sensor data.
- **SEN_ph**: Handles pH sensor data.
- **SEN_tds**: Manages Total Dissolved Solids sensor data.
- **SEN_do**: Manages Dissolved Oxygen sensor data.
- **SEN_turb**: Manages Turbidity sensor data.



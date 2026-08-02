# Solar Powered Grid Integrated EV Charging Module

An embedded systems project that integrates **solar energy** and the **electrical grid** to provide reliable charging for electric vehicles. The system continuously monitors voltage and current, displays real-time parameters on an LCD, and automatically disconnects the load under abnormal conditions to ensure safe operation.

---

## Features

- Solar-powered EV charging system
- Automatic switching between solar and grid power
- Real-time voltage monitoring
- Real-time current monitoring
- Overcurrent protection
- Under-voltage protection
- Over-voltage protection
- Relay-based load control
- Buzzer alarm for fault conditions
- 16×2 LCD status display
- Serial monitoring for debugging

---

## Hardware Components

- Arduino Uno
- ACS712 Current Sensor
- Voltage Sensor Module
- DHT11 Temperature & Humidity Sensor
- Relay Module
- 16×2 LCD Display
- Buzzer
- Solar Panel
- IoT Module
- Power Supply
- Connecting Wires

---

## Software Used

- Arduino IDE
- Embedded C
- LiquidCrystal Library
- ACS712 Library

---

## Block Diagram

The following block diagram shows the overall architecture of the system.

<p align="center">
  <img src="images/block_diagram.png" alt="Block Diagram" width="700">
</p>

---

## Prototype

<p align="center">
  <img src="images/prototype.jpg" alt="Prototype" width="600">
</p>

---

## Project Structure

```text
Solar-Powered-Grid-Integrated-EV-Charging-Module/
│── README.md
│── LICENSE
│
├── src/
│   └── Smart_Grid_Load_Monitor.ino
│
├── images/
│   ├── block_diagram.png
│   ├── prototype.jpg
│   └── circuit_diagram.png
│
├── docs/
│   └── Project_Presentation.pptx
│
└── hardware/
    ├── Components_List.md
    └── Pin_Connections.md
```

---

## Working Principle

1. The solar panel generates electrical energy.
2. Voltage and current sensors continuously monitor the system.
3. The Arduino Uno processes all sensor data.
4. If voltage or current exceeds predefined safety limits, the relay disconnects the load.
5. A buzzer alerts the user during fault conditions.
6. System parameters are displayed on the 16×2 LCD.
7. The IoT module can be used for remote monitoring.

---

## Source Code

The complete Arduino source code is available in the `src/` directory.

---

## Future Improvements

- MPPT-based solar charging
- Mobile application for remote monitoring
- Cloud-based IoT dashboard
- Battery State-of-Charge monitoring
- Fast EV charging support
- AI-based energy management

---

## Author

**Sriram Chodavaram**

B.Tech – Electronics and Communication Engineering

---

## License

This project is intended for educational and learning purposes.

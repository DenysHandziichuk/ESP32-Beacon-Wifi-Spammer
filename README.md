# ESP32 Wi-Fi Frame Simulator (Educational Only)

This project demonstrates how to construct and transmit **custom Wi-Fi 802.11 management frames**, specifically beacon frames, using the ESP32 and ESP‑IDF.  
The purpose of this repository is to help learners understand **low‑level Wi‑Fi protocols**, frame structures, and ESP32 raw packet transmission in a **legal, controlled, and safe** environment.

> ⚠️ **Important Notice**  
> This project is intended strictly for **educational, security research, and protocol-analysis purposes**.  
> Do **not** use this software to interfere with real networks, impersonate access points, cause disruption, or violate any laws.  
> You are fully responsible for how you use this code.

---

## 📘 What This Project Covers

This repository provides a practical introduction to:

### 🔸 802.11 Wi-Fi Architecture
- How Wi-Fi devices communicate at the frame level  
- Beacon frames and their purpose  
- Management frames vs control frames vs data frames  

### 🔸 Building Custom Beacon Frames
- Frame Control field  
- Addressing fields (DA, SA, BSSID)  
- Sequence Control  
- Information Elements (IEs)  
- SSID, supported rates, DS parameter sets  

### 🔸 Transmitting Raw Frames on ESP32
- Using ESP-IDF Wi‑Fi stack in promiscuous/raw mode  
- Constructing byte-level 802.11 frames in C  
- Sending beacon frames at custom intervals  

---

## 🚀 Requirements

To use this project, you need:

- **ESP32 development board** (WROOM, WROVER, DevKitC, etc.)  
- **ESP-IDF** (5.x or latest recommended version)  
- **Python 3.x** (for ESP-IDF tools)  
- Basic understanding of:
  - C programming  
  - Wi-Fi fundamentals  
  - ESP32 toolchain  

---

## 📂 Project Structure
/src
├── main.c # Builds and transmits custom Wi-Fi frames
├── beacon_frame.h # (Optional) Frame definitions and helpers
└── ...
/components
└── ...
/CMakeLists.txt
/README.md

# Edge-to-Cloud IoT Monitoring System (LPC2148)

## 📌 Overview
This project implements a **real-time Edge-to-Cloud IoT monitoring system** using the **LPC2148 ARM7 microcontroller**.  
It measures **temperature and humidity**, displays them locally, compares temperature against a **user-defined set-point**, and uploads data to the **ThingSpeak cloud** using an **ESP8266 (ESP-01) Wi-Fi module**.

The system uses **RTC-based hardware timing**, **external interrupts**, and **EEPROM-backed configuration**, making it reliable, accurate, and suitable for real-world embedded IoT applications.

---

## 🎯 Key Features
- 🌡️ Real-time **Temperature & Humidity monitoring** (DHT11)
- ⏱️ **RTC-based timing control** (no software delay dependency)
- ☁️ **Cloud upload** using ESP8266 + ThingSpeak
- 🔔 **Buzzer alert** when temperature exceeds set-point
- 💾 **EEPROM storage** for non-volatile set-point retention
- ⌨️ **External interrupt + keypad** for dynamic set-point update
- 📊 **Persistent alert logging** to cloud (Field 3)
- 🧠 Edge processing to reduce unnecessary cloud traffic

---

## 🧩 System Architecture
**Edge Device → Wi-Fi Module → Cloud Platform**

- LPC2148 acts as the **edge controller**
- ESP8266 handles **Wi-Fi & TCP/IP**
- ThingSpeak stores and visualizes sensor data

---

## 🧱 Hardware Components
- LPC2148 ARM7 Microcontroller  
- DHT11 Temperature & Humidity Sensor  
- ESP8266 (ESP-01) Wi-Fi Module  
- 16×2 LCD Display  
- Matrix Keypad  
- Buzzer  
- I2C EEPROM  
- Power Supply  

---

## 🔌 Communication Interfaces
| Peripheral | Interface |
|----------|-----------|
| ESP8266 | UART |
| EEPROM | I2C |
| RTC | Internal |
| DHT11 | GPIO |
| LCD | GPIO |
| Keypad | GPIO + External Interrupt |

---

## ☁️ ThingSpeak Field Mapping
| Field | Data |
|------|------|
| Field 1 | Temperature |
| Field 2 | Humidity |
| Field 3 | Alert Temperature (updated only when threshold is crossed) |

---

## ⚙️ Working Principle
1. On startup, the system initializes peripherals and reads the **set-point from EEPROM**
2. DHT11 sensor provides temperature & humidity values
3. Values are displayed on the LCD
4. RTC minute register controls **periodic cloud updates**
5. If temperature exceeds set-point:
   - Buzzer is activated
   - Alert temperature is stored and uploaded to Field 3
6. External interrupt allows the user to **update set-point using keypad**
7. Updated set-point is stored back into EEPROM

---

## 🛠️ Tools & Technologies
- Embedded C
- Keil µVision
- LPC2148 ARM7
- ESP8266 AT Commands
- ThingSpeak Cloud

---

## 🚀 Future Enhancements
- Timestamped cloud logging
- Mobile app dashboard
- Multiple sensor support
- OTA firmware updates

---

## 👩‍💻 Author
**Ayesha Sheik**  

---

## 📌 Note
Wi-Fi credentials and API keys are intentionally masked for security.

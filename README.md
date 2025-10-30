# IS2101-Interrupts--NNM24IS194-
#  Interrupt Simulation in C++

A multithreaded C++ simulation of a simple **interrupt controller** handling interrupts from multiple devices with different priorities.

---

##  Overview
This project demonstrates how interrupts are queued, prioritized, masked, and handled using modern C++ features.

**Devices:**
- Keyboard → High Priority  
- Mouse → Medium Priority  
- Printer → Low Priority  

Each device runs in its own thread, periodically generating interrupts for the controller to process.

---

##  Features
- Priority-based interrupt handling (Keyboard > Mouse > Printer)  
- Thread-safe interrupt queue  
- Mask/Unmask commands for each device  
- Console and file logging (`isr_log.txt`)  
- Timestamped event tracking  

---

##  Build & Run

### Compile
```bash
g++ -std=c++17 -pthread InterruptSimulation.cpp -o InterruptSimulation
./InterruptSimulation
| Command           | Description                      |
| ----------------- | -------------------------------- |
| `mask <device>`   | Disable interrupts from a device |
| `unmask <device>` | Re-enable interrupts             |
| `status`          | Show mask status and log count   |
| `exit`            | Stop the simulation              |
2025-10-30 18:22:45 | Keyboard | HANDLED
2025-10-30 18:22:48 | Printer | IGNORED (MASKED)

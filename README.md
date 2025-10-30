# IS2101-Interrupts--NNM24IS194-

#  Interrupt Controller Simulation (C++)

##  Overview
This project simulates the working of an **Interrupt Controller** using **C++ multithreading**.  
It demonstrates how interrupts from multiple I/O devices (**Keyboard**, **Mouse**, and **Printer**) are generated, prioritized, masked, and handled by a central controller in real-time.

The program uses **threads**, **mutexes**, **condition variables**, and **priority queues** to mimic concurrent device behavior in an operating system environment.

---

##  Features

-  **Priority-based interrupt handling**
  - Keyboard → High priority  
  - Mouse → Medium priority  
  - Printer → Low priority  

-  **Masking and unmasking**
  - Temporarily disable interrupt handling for any device using runtime commands.

-  **Real-time logging**
  - Logs all handled and ignored interrupts into `isr_log.txt` with timestamps.

-  **Multithreaded simulation**
  - Separate threads for each device generating interrupts asynchronously.
  - Controlled interrupt handling thread ensures realistic timing and synchronization.

-  **Interactive Command Console**
  ```bash
  mask <device>     # Mask a device (keyboard/mouse/printer)
  unmask <device>   # Unmask a device
  status            # Show current mask status and log summary
  exit              # Stop the simulation
````

---

##  How It Works

1. Each device runs in its own thread, periodically generating interrupt events.
2. The `InterruptController` maintains a **priority queue** for pending interrupts.
3. When an interrupt arrives:

   * If the device is **masked**, it’s **ignored** and logged as “IGNORED (MASKED)”.
   * Otherwise, it’s **handled** (simulated ISR execution) and logged as “HANDLED”.
4. The user can dynamically mask/unmask devices during runtime using commands.

---

##  Technologies Used

* **Language:** C++17
* **Libraries:**

  * `<thread>` – for concurrent device simulation
  * `<mutex>` & `<condition_variable>` – for thread synchronization
  * `<queue>` – for priority management
  * `<chrono>` – for timing and delays
  * `<fstream>` – for file logging
  * `<map>`, `<random>`, `<iomanip>`, `<sstream>` – for data structures and formatting

---

##  How to Run

### **1. Clone the repository**

```bash
git clone https://github.com/<your-username>/interrupt-controller-simulation.git
cd interrupt-controller-simulation
```

### **2. Compile**

Using `g++`:

```bash
g++ -std=c++17 interrupt_controller.cpp -o interrupt_controller -pthread
```

### **3. Run the Simulation**

```bash
./interrupt_controller
```

---

## 🗂️ Log File Example (`isr_log.txt`)

```text
2025-10-30 15:23:18 | Keyboard | HANDLED
2025-10-30 15:23:19 | Mouse | IGNORED (MASKED)
2025-10-30 15:23:22 | Printer | HANDLED
```

---

## 📊 File Structure

```
interrupt-controller-simulation/
│
├── interrupt_controller.cpp   # Main C++ source code
├── isr_log.txt                # Log file (auto-generated)
└── README.md                  # Project documentation
```

```

---

## ✨ Author

**Shreya Salian**
B.Tech – NMAM Institute of Technology
USN: NNM24IS194
📧 Email: [nnm24is194@nmamit.in](mailto:nnm24is194@nmamit.in)

```

---




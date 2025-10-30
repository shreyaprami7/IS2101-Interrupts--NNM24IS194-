# IS2101-Interrupts--NNM24IS194-
#  Interrupt Controller Simulation (C++)

##  Overview
This project simulates the working of an **Interrupt Controller** using **C++ multithreading**.  
It demonstrates how interrupts from multiple I/O devices (Keyboard, Mouse, Printer) are generated, prioritized, masked, and handled by a central controller in real-time.

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
🏗️ How It Works

Each device runs in its own thread, periodically generating interrupt events.

The InterruptController maintains a priority queue for pending interrupts.

When an interrupt arrives:

If the device is masked, it’s ignored and logged as “IGNORED (MASKED)”.

Otherwise, it’s handled (simulated ISR execution) and logged as “HANDLED”.

The user can dynamically mask/unmask devices during runtime using commands.
Technologies Used

Language: C++17

Libraries:

<thread> – for concurrent device simulation

<mutex> & <condition_variable> – for thread synchronization

<queue> – for priority management

<chrono> – for timing and delays

<fstream> – for file logging
git clone https://github.com/<your-username>/interrupt-controller-simulation.git
cd interrupt-controller-simulation
g++ -std=c++17 interrupt_controller.cpp -o interrupt_controller -pthread
./interrupt_controller


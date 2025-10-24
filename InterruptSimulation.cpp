#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>
#include <fstream>
#include <map>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;


enum class Device {
    KEYBOARD = 1, // High
    MOUSE = 2,    // Medium
    PRINTER = 3   // Low
};

struct InterruptEvent {
    Device device;
    long id;
    chrono::system_clock::time_point createdAt;
};

struct InterruptCompare {
    bool operator()(const InterruptEvent& a, const InterruptEvent& b) const {
        return static_cast<int>(a.device) > static_cast<int>(b.device);
    }
};

string timestamp() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    localtime_r(&t, &local);
#endif
    ostringstream oss;
    oss << put_time(&local, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

string deviceName(Device d) {
    switch (d) {
        case Device::KEYBOARD: return "Keyboard";
        case Device::MOUSE: return "Mouse";
        case Device::PRINTER: return "Printer";
    }
    return "Unknown";
}

class InterruptController {
    priority_queue<InterruptEvent, vector<InterruptEvent>, InterruptCompare> queue;
    mutex mtx;
    condition_variable cv;
    atomic<bool> running{true};
    map<Device, atomic<bool>> masked;
    vector<string> history;
    ofstream logFile;

public:
    InterruptController() {
        masked[Device::KEYBOARD] = false;
        masked[Device::MOUSE] = false;
        masked[Device::PRINTER] = false;
        logFile.open("isr_log.txt", ios::app);
    }

    ~InterruptController() {
        if (logFile.is_open()) logFile.close();
    }

    void submitInterrupt(const InterruptEvent& ev) {
        {
            lock_guard<mutex> lock(mtx);
            queue.push(ev);
        }
        cout << deviceName(ev.device) << " Interrupt Queued (id=" << ev.id << ")\n";
        cv.notify_one();
    }

    void mask(Device d) { masked[d] = true; }
    void unmask(Device d) { masked[d] = false; }
    bool isMasked(Device d) { return masked[d]; }

    void stop() {
        running = false;
        cv.notify_all();
    }

    void run() {
        while (running || !queue.empty()) {
            unique_lock<mutex> lock(mtx);
            cv.wait_for(lock, chrono::milliseconds(500), [&]() { return !queue.empty() || !running; });

            if (queue.empty()) continue;

            InterruptEvent ev = queue.top();
            queue.pop();
            lock.unlock();

            Device d = ev.device;

            if (isMasked(d)) {
                string msg = deviceName(d) + " Interrupt Ignored (Masked)";
                cout << msg << endl;
                log(ev, "IGNORED (MASKED)");
                continue;
            }

            cout << deviceName(d) << " Interrupt Triggered → Handling ISR → Completed\n";
            log(ev, "HANDLED");
            this_thread::sleep_for(chrono::milliseconds(150));
        }
        cout << "InterruptController: Shutting down.\n";
    }

    void log(const InterruptEvent& ev, const string& result) {
        string entry = timestamp() + " | " + deviceName(ev.device) + " | " + result;
        {
            lock_guard<mutex> lock(mtx);
            history.push_back(entry);
            if (logFile.is_open()) logFile << entry << endl;
        }
    }

    void printStatus() {
        cout << "Masks:\n";
        for (auto& [d, state] : masked) {
            cout << "  " << deviceName(d) << " => " << (state ? "MASKED" : "ENABLED") << endl;
        }
        cout << "Execution history entries: " << history.size() << endl;
    }
};

class DeviceGenerator {
    Device device;
    InterruptController& controller;
    atomic<bool> running{true};
    long counter = 0;
    thread worker;
    mt19937 rng{random_device{}()};

public:
    DeviceGenerator(Device d, InterruptController& c) : device(d), controller(c) {}

    void start() {
        worker = thread([this]() {
            uniform_int_distribution<int> jitter(100, 500);
            while (running) {
                int baseSleep;
                switch (device) {
                    case Device::KEYBOARD: baseSleep = 300; break;
                    case Device::MOUSE: baseSleep = 700; break;
                    case Device::PRINTER: baseSleep = 1200; break;
                }
                int sleepTime = baseSleep + jitter(rng);
                this_thread::sleep_for(chrono::milliseconds(sleepTime));

                if (!running) break;

                InterruptEvent ev{device, ++counter, chrono::system_clock::now()};
                controller.submitInterrupt(ev);
            }
            cout << deviceName(device) << " generator stopped.\n";
        });
    }

    void stop() {
        running = false;
    }

    void join() {
        if (worker.joinable()) worker.join();
    }
};

int main() {
    InterruptController controller;

    DeviceGenerator keyboard(Device::KEYBOARD, controller);
    DeviceGenerator mouse(Device::MOUSE, controller);
    DeviceGenerator printer(Device::PRINTER, controller);

    keyboard.start();
    mouse.start();
    printer.start();

    thread controllerThread(&InterruptController::run, &controller);

    cout << "Commands: mask/unmask <keyboard|mouse|printer>, status, exit\n";

    string cmd;
    while (true) {
        cout << "> ";
        getline(cin, cmd);

        if (cmd == "exit" || cmd == "quit") {
            cout << "Exiting...\n";
            break;
        } else if (cmd == "status") {
            controller.printStatus();
        } else if (cmd.rfind("mask", 0) == 0) {
            if (cmd.find("keyboard") != string::npos)
                controller.mask(Device::KEYBOARD);
            else if (cmd.find("mouse") != string::npos)
                controller.mask(Device::MOUSE);
            else if (cmd.find("printer") != string::npos)
                controller.mask(Device::PRINTER);
        } else if (cmd.rfind("unmask", 0) == 0) {
            if (cmd.find("keyboard") != string::npos)
                controller.unmask(Device::KEYBOARD);
            else if (cmd.find("mouse") != string::npos)
                controller.unmask(Device::MOUSE);
            else if (cmd.find("printer") != string::npos)
                controller.unmask(Device::PRINTER);
        } else {
            cout << "Unknown command. Try: mask/unmask <device>, status, exit\n";
        }
    }

    keyboard.stop();
    mouse.stop();
    printer.stop();
    controller.stop();

    keyboard.join();
    mouse.join();
    printer.join();
    controllerThread.join();

    cout << "Simulation finished. Log appended to isr_log.txt\n";
    return 0;
}

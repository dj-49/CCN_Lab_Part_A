#include <iostream>
#include <thread>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

class Network {
    double reliability;
    mt19937 rng{random_device{}()};
    uniform_real_distribution<> dist{0.0, 1.0};

public:
    Network(double rel = 0.8) : reliability(rel) {}

    bool transmit(int frameNum, bool isAck = false) {
        cout << "Sending " << (isAck ? "ACK for " : "") << "frame " << frameNum;
        this_thread::sleep_for(milliseconds(isAck ? 300 : 500));
        bool success = dist(rng) < reliability;
        cout << " - " << (success ? "delivered\n" : "lost\n");
        return success;
    }
};

class StopAndWait {
    Network net;
    int timeout;

public:
    StopAndWait(double reliability = 0.8, int timeoutMs = 2000)
        : net(reliability), timeout(timeoutMs) {}

    void send(int totalFrames) {
        for (int frame = 0; frame < totalFrames;) {
            cout << "\n--- Frame " << frame << " ---\n";
            auto start = steady_clock::now();

            if (net.transmit(frame) && net.transmit(frame, true)) {
                cout << "Frame " << frame << " acknowledged\n";
                ++frame;
            } else if (duration_cast<milliseconds>(steady_clock::now() - start).count() > timeout) {
                cout << "Timeout - retrying\n";
            }

            this_thread::sleep_for(milliseconds(300));
        }
        cout << "\nAll " << totalFrames << " frames sent successfully\n";
    }
};

int main() {
    cout << "Stop-and-Wait Protocol Simulation\n";
    double reliability;
    int frames;

    cout << "Enter network reliability (0.0 - 1.0): ";
    cin >> reliability;
    cout << "Enter number of frames: ";
    cin >> frames;

    StopAndWait(reliability).send(frames);
    return 0;
}

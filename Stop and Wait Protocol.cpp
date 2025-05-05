#include <iostream>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

class Network {
private:
    double reliability;
    mt19937 rng{time(nullptr)};
    uniform_real_distribution<double> dist{0.0, 1.0};

public:
    Network(double rel = 0.8) : reliability(rel) {}

    bool sendData(int frameNum) {
        cout << "Sending frame " << frameNum << "..." << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        bool success = dist(rng) < reliability;
        cout << "Frame " << frameNum << (success ? " delivered successfully" : " lost in transmission") << endl;
        return success;
    }

    bool sendAck(int frameNum) {
        cout << "Sending ACK for frame " << frameNum << "..." << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
        bool success = dist(rng) < reliability;
        cout << "ACK for frame " << frameNum << (success ? " delivered successfully" : " lost in transmission") << endl;
        return success;
    }
};

class StopAndWaitProtocol {
private:
    Network net;
    int timeout;

public:
    StopAndWaitProtocol(double reliability = 0.8, int timeoutMs = 2000) 
        : net(reliability), timeout(timeoutMs) {}

    void sender(int totalFrames) {
        for (int frameNum = 0; frameNum < totalFrames;) {
            cout << "\n--- SENDER: Attempting to send frame " << frameNum << " ---\n";
            auto startTime = chrono::steady_clock::now();
            bool frameDelivered = false;

            while (!frameDelivered) {
                if (net.sendData(frameNum)) {
                    cout << "Waiting for acknowledgment..." << endl;
                    if (net.sendAck(frameNum)) {
                        cout << "ACK received for frame " << frameNum << endl;
                        frameDelivered = true;
                        frameNum++;
                    } else {
                        cout << "ACK timeout, retransmitting frame " << frameNum << endl;
                    }
                } else {
                    cout << "Frame transmission failed, retransmitting..." << endl;
                }

                auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(
                    chrono::steady_clock::now() - startTime).count();

                if (!frameDelivered) {
                    if (elapsedTime > timeout) {
                        cout << "Timeout occurred after " << elapsedTime 
                             << "ms. Retransmitting frame " << frameNum << endl;
                        startTime = chrono::steady_clock::now();
                    }
                    this_thread::sleep_for(chrono::milliseconds(500));
                }
            }
        }
        cout << "\nAll " << totalFrames << " frames sent successfully!\n";
    }
};

int main() {
    cout << "Stop and Wait Protocol Demonstration\n";
    cout << "------------------------------------\n";

    double reliability;
    int frames;

    cout << "Enter network reliability (0.0-1.0): ";
    cin >> reliability;
    cout << "Enter number of frames to send: ";
    cin >> frames;

    StopAndWaitProtocol(reliability).sender(frames);
    return 0;
}

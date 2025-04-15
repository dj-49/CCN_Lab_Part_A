#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <random>  // For better random number generation

using namespace std;

// Simulates network transmission with configurable reliability
class Network {
private:
    double reliability; // Probability of successful transmission (0.0-1.0)
    mt19937 rng;        // Better random number generator
    uniform_real_distribution<double> dist;  // For generating values between 0-1

public:
    Network(double rel = 0.8) : reliability(rel), rng(time(nullptr)), dist(0.0, 1.0) {
        // Initialize random number generator with current time
    }

    // Simulate sending data with possible packet loss
    bool sendData(int frameNum) {
        cout << "Sending frame " << frameNum << "..." << endl;

        // Simulate network delay
        this_thread::sleep_for(chrono::milliseconds(500));

        // Determine if transmission is successful based on reliability
        // Using modern C++ random facilities instead of rand()
        bool success = dist(rng) < reliability;

        if (success) {
            cout << "Frame " << frameNum << " delivered successfully" << endl;
        } else {
            cout << "Frame " << frameNum << " lost in transmission" << endl;
        }

        return success;
    }

    // Simulate sending acknowledgment with possible loss
    bool sendAck(int frameNum) {
        cout << "Sending ACK for frame " << frameNum << "..." << endl;

        // Simulate network delay
        this_thread::sleep_for(chrono::milliseconds(300));

        // Determine if ACK transmission is successful based on reliability
        bool success = dist(rng) < reliability;

        if (success) {
            cout << "ACK for frame " << frameNum << " delivered successfully" << endl;
        } else {
            cout << "ACK for frame " << frameNum << " lost in transmission" << endl;
        }

        return success;
    }
};

// Implements Stop and Wait Protocol
class StopAndWaitProtocol {
private:
    Network net;
    int timeout; // Timeout in milliseconds

public:
    StopAndWaitProtocol(double reliability = 0.8, int timeoutMs = 2000) 
        : net(reliability), timeout(timeoutMs) {}

    // Sender implementation
    void sender(int totalFrames) {
        int frameNum = 0;

        while (frameNum < totalFrames) {
            cout << "\n--- SENDER: Attempting to send frame " << frameNum << " ---\n";

            auto startTime = chrono::steady_clock::now();
            bool frameDelivered = false;

            while (!frameDelivered) {
                // Send the frame
                bool frameSent = net.sendData(frameNum);

                if (frameSent) {
                    // Wait for acknowledgment
                    cout << "Waiting for acknowledgment..." << endl;

                    // Simulate the receiver processing and sending ACK
                    bool ackReceived = net.sendAck(frameNum);

                    if (ackReceived) {
                        cout << "ACK received for frame " << frameNum << endl;
                        frameDelivered = true;
                        frameNum++;  // Move to next frame
                    } else {
                        cout << "ACK timeout, retransmitting frame " << frameNum << endl;
                    }
                } else {
                    cout << "Frame transmission failed, retransmitting..." << endl;
                }

                // Check if we've exceeded the timeout
                auto currentTime = chrono::steady_clock::now();
                auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(
                    currentTime - startTime).count();

                if (!frameDelivered && elapsedTime > timeout) {
                    cout << "Timeout occurred after " << elapsedTime 
                         << "ms. Retransmitting frame " << frameNum << endl;
                    startTime = chrono::steady_clock::now();  // Reset timeout timer
                }

                // Small delay before retransmission
                if (!frameDelivered) {
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

    // Create protocol with specified reliability
    StopAndWaitProtocol protocol(reliability);

    // Start transmission
    protocol.sender(frames);

    return 0;
}

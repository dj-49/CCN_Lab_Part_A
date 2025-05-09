#include <iostream>
#include <thread>
#include <chrono>
#include <random>
using namespace std;

class Network {
private:
    double reliability;
    mt19937 rng{random_device{}()};
    uniform_real_distribution<> dist{0.0, 1.0};
    
public:
    Network(double rel = 0.8) : reliability(rel) {}
    
    bool transmit(int frameNum, bool isAck = false) {
        cout << "Sending " << (isAck ? "ACK for frame " : "frame ") << frameNum;
        this_thread::sleep_for(chrono::milliseconds(isAck ? 300 : 500));
        bool success = dist(rng) < reliability;
        cout << " - " << (success ? "delivered" : "lost") << endl;
        return success;
    }
    
    bool sendData(int frameNum) { return transmit(frameNum); }
    bool sendAck(int frameNum) { return transmit(frameNum, true); }
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
            cout << "\n--- Frame " << frameNum << " ---\n";
            auto startTime = chrono::steady_clock::now();
            
            while (true) {
                if (net.sendData(frameNum) && net.sendAck(frameNum)) {
                    cout << "Frame " << frameNum << " successfully sent and acknowledged\n";
                    frameNum++;
                    break;
                }
                
                // Check for timeout
                auto elapsed = chrono::duration_cast<chrono::milliseconds>(
                    chrono::steady_clock::now() - startTime).count();
                    
                if (elapsed > timeout) {
                    cout << "Timeout - retrying\n";
                    startTime = chrono::steady_clock::now();
                }
                
                this_thread::sleep_for(chrono::milliseconds(300));
            }
        }
        cout << "\nAll " << totalFrames << " frames sent successfully\n";
    }
};

int main() {
    cout << "Stop-and-Wait Protocol Demo\n";
    
    double reliability = 0.8;
    int frames = 5;
    
    cout << "Network reliability (0.0-1.0): ";
    cin >> reliability;
    cout << "Number of frames: ";
    cin >> frames;
    
    StopAndWaitProtocol(reliability).sender(frames);
    return 0;
}

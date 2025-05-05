#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

class SlidingWindowProtocol {
private:
    int windowSize, totalPackets, base = 0, nextSeqNum = 0;
    vector<bool> ackReceived;
    queue<int> network;
    default_random_engine generator{chrono::system_clock::now().time_since_epoch().count()};
    bernoulli_distribution packetLoss;

public:
    SlidingWindowProtocol(int wSize, int packets, double lossProb) : 
        windowSize(wSize), totalPackets(packets), packetLoss(lossProb) {
        ackReceived.resize(totalPackets, false);
    }
    
    void sendPackets() {
        while (base < totalPackets) {
            // Send packets within window
            while (nextSeqNum < base + windowSize && nextSeqNum < totalPackets) {
                cout << "Sending packet " << nextSeqNum << endl;
                
                if (!packetLoss(generator)) {
                    network.push(nextSeqNum);
                } else {
                    cout << "  (Packet " << nextSeqNum << " was lost in transmission)" << endl;
                }
                
                nextSeqNum++;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            
            receiveAcks();
            
            if (nextSeqNum == base + windowSize || nextSeqNum == totalPackets) {
                cout << "Window full or all packets sent. Waiting for ACKs..." << endl;
                receiveAcks();
            }
        }
        
        cout << "All packets have been successfully transmitted and acknowledged." << endl;
    }
    
    void receiveAcks() {
        // Process all packets in the network
        while (!network.empty()) {
            int packetNum = network.front();
            network.pop();
            
            cout << "Receiver got packet " << packetNum << endl;
            
            if (!packetLoss(generator)) {
                cout << "  ACK " << packetNum << " sent back to sender" << endl;
                ackReceived[packetNum] = true;
                
                // Update base (slide window)
                while (base < totalPackets && ackReceived[base])
                    base++;
                
                cout << "  Window slides to [" << base << " - " 
                     << min(base + windowSize - 1, totalPackets - 1) << "]" << endl;
            } else {
                cout << "  ACK " << packetNum << " was lost" << endl;
            }
            
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        
        // Retransmit unacknowledged packets
        for (int i = base; i < nextSeqNum; i++) {
            if (!ackReceived[i]) {
                cout << "Timeout for packet " << i << ", retransmitting..." << endl;
                
                if (!packetLoss(generator)) {
                    network.push(i);
                } else {
                    cout << "  (Retransmitted packet " << i << " was lost)" << endl;
                }
                
                this_thread::sleep_for(chrono::milliseconds(150));
            }
        }
    }
};

int main() {
    cout << "Sliding Window Protocol Demonstration" << endl;
    cout << "Window Size: 3" << endl;
    cout << "Total Packets: 9" << endl;
    cout << "Simulated Packet Loss Probability: 0.3" << endl;
    cout << "-------------------------------------------" << endl;
    
    SlidingWindowProtocol(4, 9, 0.3).sendPackets();
    return 0;
}

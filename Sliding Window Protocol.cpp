#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

class SlidingWindowProtocol {
private:
    int windowSize;           // Size of the sliding window
    int totalPackets;         // Total number of packets to send
    int base;                 // Base sequence number of the window
    int nextSeqNum;           // Next sequence number to be used
    vector<bool> ackReceived; // Track acknowledgements
    queue<int> network;       // Simulate network for packet transmission
    
    // Random number generator for packet loss simulation
    default_random_engine generator;
    bernoulli_distribution packetLoss;

public:
    SlidingWindowProtocol(int wSize, int packets, double lossProb) : 
        windowSize(wSize), 
        totalPackets(packets),
        base(0), 
        nextSeqNum(0),
        packetLoss(lossProb) {
        
        // Initialize with all packets unacknowledged
        ackReceived.resize(totalPackets, false);
        
        // Seed the random number generator
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
    
    void sendPackets() {
        while (base < totalPackets) {
            // Send packets within the window that haven't been sent yet
            while (nextSeqNum < base + windowSize && nextSeqNum < totalPackets) {
                cout << "Sending packet " << nextSeqNum << endl;
                
                // Simulate packet loss
                if (!packetLoss(generator)) {
                    network.push(nextSeqNum);
                } else {
                    cout << "  (Packet " << nextSeqNum << " was lost in transmission)" << endl;
                }
                
                nextSeqNum++;
                
                // Small delay for demonstration purposes
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            
            // Process acknowledgments
            receiveAcks();
            
            // If window is full or all packets sent, wait for acks
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
            
            // Simulate packet processing at receiver
            cout << "Receiver got packet " << packetNum << endl;
            
            // Send ACK (might be lost)
            if (!packetLoss(generator)) {
                cout << "  ACK " << packetNum << " sent back to sender" << endl;
                ackReceived[packetNum] = true;
                
                // Update base (slide window)
                while (base < totalPackets && ackReceived[base]) {
                    base++;
                }
                
                cout << "  Window slides to [" << base << " - " 
                     << min(base + windowSize - 1, totalPackets - 1) << "]" << endl;
            } else {
                cout << "  ACK " << packetNum << " was lost" << endl;
            }
            
            // Small delay for demonstration purposes
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        
        // Implement retransmission timeout
        // Check for unacknowledged packets within the window
        for (int i = base; i < nextSeqNum; i++) {
            if (!ackReceived[i]) {
                cout << "Timeout for packet " << i << ", retransmitting..." << endl;
                
                // Retransmit the packet
                if (!packetLoss(generator)) {
                    network.push(i);
                } else {
                    cout << "  (Retransmitted packet " << i << " was lost)" << endl;
                }
                
                // Small delay between retransmissions
                this_thread::sleep_for(chrono::milliseconds(150));
            }
        }
    }
};

int main() {
    int windowSize = 4;           // Size of the sliding window
    int totalPackets = 15;        // Total number of packets to send
    double packetLossProb = 0.3;  // Probability of packet loss
    
    cout << "Sliding Window Protocol Demonstration" << endl;
    cout << "Window Size: " << windowSize << endl;
    cout << "Total Packets: " << totalPackets << endl;
    cout << "Simulated Packet Loss Probability: " << packetLossProb << endl;
    cout << "-------------------------------------------" << endl;
    
    SlidingWindowProtocol swp(windowSize, totalPackets, packetLossProb);
    swp.sendPackets();
    
    return 0;
}

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <thread>
using namespace std;

class SlidingWindow {
private:
    int windowSize, totalPackets, base = 0, nextSeq = 0;
    vector<bool> acked;
    queue<int> network;
    mt19937 rng{random_device{}()};
    bernoulli_distribution lossChance;
    
    bool sendWithLoss(int pktNum) {
        if (!lossChance(rng)) {
            cout << "Sent packet " << pktNum << endl;
            network.push(pktNum);
            return true;
        }
        cout << "Lost packet " << pktNum << endl;
        return false;
    }
    
public:
    SlidingWindow(int wSize, int packets, double lossProb) : 
        windowSize(wSize), totalPackets(packets), 
        acked(packets, false), lossChance(lossProb) {}
    
    void simulate() {
        while (base < totalPackets) {
            // Send packets within window
            while (nextSeq < base + windowSize && nextSeq < totalPackets) {
                sendWithLoss(nextSeq++);
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            
            processAcks();
            
            if (nextSeq == base + windowSize || nextSeq == totalPackets) {
                cout << "Window: [" << base << "-" << min(base + windowSize - 1, totalPackets - 1) << "]" << endl;
                processAcks();
            }
        }
        cout << "All packets transmitted successfully." << endl;
    }
    
    void processAcks() {
        // Process packets in network
        while (!network.empty()) {
            int pkt = network.front();
            network.pop();
            
            if (!lossChance(rng)) {
                cout << "ACK " << pkt << " received" << endl;
                acked[pkt] = true;
                
                // Slide window
                while (base < totalPackets && acked[base]) base++;
                if (base > 0)
                    cout << "Window slides to [" << base << "-" 
                         << min(base + windowSize - 1, totalPackets - 1) << "]" << endl;
            } else {
                cout << "ACK " << pkt << " lost" << endl;
            }
            
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        
        // Retransmit unacknowledged packets
        for (int i = base; i < nextSeq; i++) {
            if (!acked[i]) {
                cout << "Timeout: retransmitting " << i << endl;
                sendWithLoss(i);
            }
        }
    }
};

int main() {
    cout << "Sliding Window Protocol Demo\n";
    cout << "Window Size: 4, Packets: 9, Loss Prob: 0.3\n";
    cout << "------------------------------\n";
    
    SlidingWindow(4, 9, 0.3).simulate();
    return 0;
}

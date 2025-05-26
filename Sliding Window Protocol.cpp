#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

class SlidingWindow {
    int windowSize, totalPackets, base = 0, nextSeq = 0;
    vector<bool> acked;
    queue<int> network;
    mt19937 rng{random_device{}()};
    bernoulli_distribution loss;

    bool sendPacket(int pkt) {
        if (loss(rng)) {
            cout << "Lost packet " << pkt << endl;
            return false;
        }
        cout << "Sent packet " << pkt << endl;
        network.push(pkt);
        return true;
    }

    void handleAcks() {
        while (!network.empty()) {
            int pkt = network.front(); network.pop();
            if (loss(rng)) {
                cout << "ACK " << pkt << " lost\n";
                continue;
            }

            cout << "ACK " << pkt << " received\n";
            acked[pkt] = true;
            while (base < totalPackets && acked[base]) base++;
        }
    }

    void retransmit() {
        for (int i = base; i < nextSeq; ++i) {
            if (!acked[i]) {
                cout << "Timeout: retransmitting " << i << endl;
                sendPacket(i);
            }
        }
    }

public:
    SlidingWindow(int w, int total, double lossProb)
        : windowSize(w), totalPackets(total), acked(total, false), loss(lossProb) {}

    void simulate() {
        while (base < totalPackets) {
            while (nextSeq < base + windowSize && nextSeq < totalPackets) {
                sendPacket(nextSeq++);
                this_thread::sleep_for(milliseconds(100));
            }

            cout << "Window: [" << base << "-" << min(base + windowSize - 1, totalPackets - 1) << "]\n";
            handleAcks();
            retransmit();
            this_thread::sleep_for(milliseconds(100));
        }
        cout << "All packets transmitted successfully.\n";
    }
};

int main() {
    cout << "Sliding Window Protocol Demo\nWindow Size: 4, Packets: 9, Loss Prob: 0.3\n";
    cout << "------------------------------\n";
    SlidingWindow(4, 9, 0.3).simulate();
    return 0;
}

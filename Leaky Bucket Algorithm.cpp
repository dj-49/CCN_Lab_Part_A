#include <iostream>
#include <queue>
#include <thread>
#include <random>
#include <iomanip>

using namespace std;

class LeakyBucket {
private:
    queue<int> bucket;
    int bucketCapacity, outflowRate;
    int totalPacketsArrived = 0, totalPacketsTransmitted = 0, totalPacketsDropped = 0;
    default_random_engine generator{chrono::system_clock::now().time_since_epoch().count()};
    poisson_distribution<int> packetArrival;

public:
    LeakyBucket(int capacity, int rate, double arrivalMean) : 
        bucketCapacity(capacity), outflowRate(rate), packetArrival(arrivalMean) {}

    void simulateNetworkTraffic(int simulationTime) {
        cout << "Leaky Bucket Algorithm Simulation\n"
             << "Bucket Capacity: " << bucketCapacity << " packets\n"
             << "Outflow Rate: " << outflowRate << " packets/second\n\n"
             << "Simulation started for " << simulationTime << " seconds...\n"
             << left << setw(10) << "Time(s)" << setw(15) << "Packets In" 
             << setw(15) << "Bucket Size" << setw(15) << "Packets Out" 
             << setw(15) << "Packets Dropped\n"
             << string(70, '-') << endl;

        for (int currentSecond = 1; currentSecond <= simulationTime; currentSecond++) {
            int incomingPackets = packetArrival(generator);
            totalPacketsArrived += incomingPackets;
            
            int droppedPackets = 0;
            for (int i = 0; i < incomingPackets; i++) {
                if (bucket.size() < bucketCapacity)
                    bucket.push(currentSecond);
                else
                    droppedPackets++;
            }
            totalPacketsDropped += droppedPackets;
            
            int outgoingPackets = min(outflowRate, static_cast<int>(bucket.size()));
            for (int i = 0; i < outgoingPackets; i++)
                bucket.pop();
                
            totalPacketsTransmitted += outgoingPackets;
            
            cout << left << setw(10) << currentSecond 
                 << setw(15) << incomingPackets 
                 << setw(15) << bucket.size() 
                 << setw(15) << outgoingPackets 
                 << setw(15) << droppedPackets << endl;
            
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        
        cout << "\nSimulation completed.\n"
             << "Total packets arrived: " << totalPacketsArrived << "\n"
             << "Total packets transmitted: " << totalPacketsTransmitted << "\n"
             << "Total packets dropped: " << totalPacketsDropped << "\n"
             << "Final bucket size: " << bucket.size() << " packets\n"
             << "Transmission efficiency: " << fixed << setprecision(2) 
             << (totalPacketsTransmitted * 100.0) / totalPacketsArrived << "%" << endl;
    }
};

int main() {
    LeakyBucket(10, 3, 4.0).simulateNetworkTraffic(20);
    return 0;
}

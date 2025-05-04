#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <random>
#include <iomanip>

using namespace std;

class LeakyBucket {
private:
    queue<int> bucket;           // Queue to store packets
    int bucketCapacity;          // Maximum capacity of the bucket
    int outflowRate;             // Rate at which packets leak out (packets per second)
    int totalPacketsArrived;     // Counter for total packets arrived
    int totalPacketsTransmitted; // Counter for total packets transmitted
    int totalPacketsDropped;     // Counter for total packets dropped

    // Random number generator for packet arrival simulation
    default_random_engine generator;
    poisson_distribution<int> packetArrival;

public:
    LeakyBucket(int capacity, int rate, double arrivalMean) : 
        bucketCapacity(capacity), 
        outflowRate(rate),
        totalPacketsArrived(0),
        totalPacketsTransmitted(0),
        totalPacketsDropped(0),
        packetArrival(arrivalMean) {
        
        // Seed the random number generator
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    void simulateNetworkTraffic(int simulationTime) {
        cout << "Leaky Bucket Algorithm Simulation" << endl;
        cout << "Bucket Capacity: " << bucketCapacity << " packets" << endl;
        cout << "Outflow Rate: " << outflowRate << " packets/second" << endl;
        cout << "\nSimulation started for " << simulationTime << " seconds..." << endl;
        
        cout << left << setw(10) << "Time(s)" 
             << setw(15) << "Packets In" 
             << setw(15) << "Bucket Size" 
             << setw(15) << "Packets Out" 
             << setw(15) << "Packets Dropped" << endl;
        
        cout << string(70, '-') << endl;

        // Simulate network traffic for the specified duration
        for (int currentSecond = 1; currentSecond <= simulationTime; currentSecond++) {
            // Generate random number of incoming packets based on Poisson distribution
            int incomingPackets = packetArrival(generator);
            totalPacketsArrived += incomingPackets;
            
            // Calculate dropped packets
            int droppedPackets = 0;
            for (int i = 0; i < incomingPackets; i++) {
                if (bucket.size() < bucketCapacity) {
                    bucket.push(currentSecond); // Store timestamp for demonstration
                } else {
                    droppedPackets++;
                }
            }
            totalPacketsDropped += droppedPackets;
            
            // Process outgoing packets at the specified rate
            int outgoingPackets = min(outflowRate, static_cast<int>(bucket.size()));
            for (int i = 0; i < outgoingPackets; i++) {
                bucket.pop();
            }
            totalPacketsTransmitted += outgoingPackets;
            
            // Print status at this time step
            cout << left << setw(10) << currentSecond 
                 << setw(15) << incomingPackets 
                 << setw(15) << bucket.size() 
                 << setw(15) << outgoingPackets 
                 << setw(15) << droppedPackets << endl;
            
            // Simulate passage of time
            this_thread::sleep_for(chrono::milliseconds(100)); // Speed up simulation
        }
        
        // Print final statistics
        cout << "\nSimulation completed." << endl;
        cout << "Total packets arrived: " << totalPacketsArrived << endl;
        cout << "Total packets transmitted: " << totalPacketsTransmitted << endl;
        cout << "Total packets dropped: " << totalPacketsDropped << endl;
        cout << "Final bucket size: " << bucket.size() << " packets" << endl;
        
        // Calculate efficiency
        double efficiency = (totalPacketsTransmitted * 100.0) / totalPacketsArrived;
        cout << "Transmission efficiency: " << fixed << setprecision(2) << efficiency << "%" << endl;
    }
};

int main() {
    int bucketCapacity = 10;     // Maximum capacity of the bucket in packets
    int outflowRate = 3;         // Rate at which packets leak out (packets per second)
    double arrivalMean = 4.0;    // Mean arrival rate (packets per second) - Poisson distribution
    int simulationTime = 20;     // Simulation duration in seconds
    
    LeakyBucket leakyBucket(bucketCapacity, outflowRate, arrivalMean);
    leakyBucket.simulateNetworkTraffic(simulationTime);
    
    return 0;
}

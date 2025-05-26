#include <iostream>
#include <queue>
#include <thread>
#include <random>
#include <iomanip>
using namespace std;

class LeakyBucket {
    int cap, rate, inTotal = 0, outTotal = 0, dropTotal = 0;
    queue<int> bucket;
    default_random_engine gen{static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count())};
    poisson_distribution<> arrival;

public:
    LeakyBucket(int c, int r, double mean) : cap(c), rate(r), arrival(mean) {}

    void simulate(int seconds) {
        cout << "Leaky Bucket Simulation\nCapacity: " << cap 
             << ", Rate: " << rate << " pkt/s, Duration: " << seconds << "s\n\n"
             << left << setw(8) << "Time" << setw(12) << "In" 
             << setw(12) << "Bucket" << setw(12) << "Out" 
             << setw(12) << "Dropped" << "\n" << string(56, '-') << "\n";

        for (int t = 1; t <= seconds; ++t) {
            int in = arrival(gen);
            int drop = 0;
            inTotal += in;

            // Attempt to enqueue incoming packets
            for (int i = 0; i < in; ++i) {
                if (bucket.size() < static_cast<size_t>(cap))
                    bucket.push(1);  // Content doesn't matter
                else
                    ++drop;
            }
            dropTotal += drop;

            int out = min(rate, static_cast<int>(bucket.size()));
            for (int i = 0; i < out; ++i) bucket.pop();
            outTotal += out;

            cout << setw(8) << t << setw(12) << in << setw(12) << bucket.size()
                 << setw(12) << out << setw(12) << drop << "\n";

            this_thread::sleep_for(chrono::milliseconds(100));
        }

        double efficiency = inTotal ? (outTotal * 100.0) / inTotal : 0;
        cout << "\nSummary:\nPackets In: " << inTotal 
             << ", Out: " << outTotal << ", Dropped: " << dropTotal 
             << ", Final Bucket: " << bucket.size()
             << "\nEfficiency: " << fixed << setprecision(2) << efficiency << "%\n";
    }
};

int main() {
    LeakyBucket(10, 3, 4.0).simulate(20);
    return 0;
}

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <algorithm>
using namespace std;

// Constants
const char FLAG = '~', ESC = '^';
const string BIT_FLAG = "01111110";

// Byte stuffing operations
string byteStuff(const string& data) {
    string result(1, FLAG);
    for (char c : data) {
        if (c == FLAG || c == ESC) result += ESC;
        result += c;
    }
    return result + FLAG;
}

string byteDestuff(const string& data) {
    if (data.size() < 2) return "";
    string result;
    bool escaped = false;
    
    for (size_t i = 1; i < data.length() - 1; i++) {
        if (data[i] == ESC && !escaped) escaped = true;
        else { result += data[i]; escaped = false; }
    }
    return result;
}

// Bit stuffing operations
string bitStuff(const string& binary) {
    string result = BIT_FLAG;
    int ones = 0;
    
    for (char bit : binary) {
        result += bit;
        if (bit == '1') {
            if (++ones == 5) { result += '0'; ones = 0; }
        } else ones = 0;
    }
    return result + BIT_FLAG;
}

string bitDestuff(const string& data) {
    size_t start = data.find(BIT_FLAG);
    if (start == string::npos) return "";
    
    size_t end = data.find(BIT_FLAG, start + BIT_FLAG.length());
    if (end == string::npos) return "";
    
    string payload = data.substr(start + BIT_FLAG.length(), end - start - BIT_FLAG.length());
    string result;
    int ones = 0;
    
    for (size_t i = 0; i < payload.length(); i++) {
        result += payload[i];
        if (payload[i] == '1') {
            if (++ones == 5 && i + 1 < payload.length() && payload[i + 1] == '0') {
                i++; ones = 0;
            }
        } else ones = 0;
    }
    return result;
}

// Helper functions
string toBinary(const string& data) {
    string binary;
    for (char c : data)
        binary += bitset<8>(c).to_string();
    return binary;
}

void printBinary(const string& binary) {
    for (size_t i = 0; i < binary.length(); i++) {
        cout << binary[i];
        if ((i + 1) % 8 == 0) cout << " ";
    }
    cout << endl;
}

vector<string> createFrames(const string& data, const vector<int>& sizes) {
    vector<string> frames;
    size_t pos = 0;
    
    for (int size : sizes) {
        if (pos >= data.length()) break;
        size_t len = min(data.length() - pos, static_cast<size_t>(size));
        frames.push_back(data.substr(pos, len));
        pos += len;
    }
    
    if (pos < data.length())
        frames.push_back(data.substr(pos));
    
    return frames;
}

int main() {
    string data;
    int stuffType, frameType;
    
    cout << "=== Data Framing with Stuffing ===" << endl;
    cout << "Enter data: ";
    getline(cin, data);
    
    cout << "Select: 1.Byte Stuffing or 2.Bit Stuffing: ";
    cin >> stuffType;
    
    cout << "Select: 1.Fixed Size or 2.Variable Size Frames: ";
    cin >> frameType;
    
    vector<int> frameSizes;
    if (frameType == 1) {
        int size;
        cout << "Enter frame size: ";
        cin >> size;
        frameSizes.assign((data.length() + size - 1) / size, size);
    } else {
        int count;
        cout << "Enter number of frames: ";
        cin >> count;
        for (int i = 0; i < count; i++) {
            int size;
            cout << "Enter size for frame " << (i+1) << ": ";
            cin >> size;
            frameSizes.push_back(size);
        }
    }
    
    vector<string> frames = createFrames(data, frameSizes);
    
    if (stuffType == 1) {  // Byte Stuffing
        cout << "\n=== Byte Stuffing Results ===" << endl;
        for (size_t i = 0; i < frames.size(); i++) {
            string stuffed = byteStuff(frames[i]);
            string destuffed = byteDestuff(stuffed);
            
            cout << "\nFrame " << (i+1) << ":" << endl;
            cout << "Original: " << frames[i] << endl;
            cout << "Stuffed : " << stuffed << endl;
            cout << "Destuffed: " << destuffed << endl;
        }
    } else {  // Bit Stuffing
        cout << "\n=== Bit Stuffing Results ===" << endl;
        for (size_t i = 0; i < frames.size(); i++) {
            string binary = toBinary(frames[i]);
            string stuffed = bitStuff(binary);
            string destuffed = bitDestuff(stuffed);
            
            cout << "\nFrame " << (i+1) << ":" << endl;
            cout << "Original: " << frames[i] << " (Binary: ";
            printBinary(binary);
            cout << ")" << endl;
            cout << "Stuffed : ";
            printBinary(stuffed);
            cout << "Destuffed: ";
            printBinary(destuffed);
        }
    }
    
    return 0;
}

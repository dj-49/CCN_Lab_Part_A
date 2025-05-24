#include <iostream>
#include <vector>
#include <string>
#include <bitset>
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
    if (data.length() < 16 || data.substr(0, 8) != BIT_FLAG || data.substr(data.length() - 8) != BIT_FLAG) return "";
    string payload = data.substr(8, data.length() - 16);
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
    for (char c : data) binary += bitset<8>(c).to_string();
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
    if (pos < data.length()) frames.push_back(data.substr(pos));
    return frames;
}

// Print functions
void printByteStuffing(size_t frameNum, const string& original, const string& stuffed, const string& destuffed) {
    cout << "\nFrame " << frameNum << ":\nOriginal: " << original << "\nStuffed : " << stuffed << "\nDestuffed: " << destuffed << endl;
}

void printBitStuffing(size_t frameNum, const string& originalText, const string& originalBinary, const string& stuffed, const string& destuffed) {
    cout << "\nFrame " << frameNum << ":\nOriginal: " << originalText << " (Binary: ";
    printBinary(originalBinary);
    cout << ")\nStuffed : ";
    printBinary(stuffed);
    cout << "Destuffed: ";
    printBinary(destuffed);
}

int main() {
    string data;
    int stuffType, frameType;
    cout << "=== Data Framing with Stuffing ===\nEnter data: ";
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
        frameSizes.resize(count);
        for (int i = 0; i < count; i++) {
            cout << "Enter size for frame " << (i + 1) << ": ";
            cin >> frameSizes[i];
        }
    }

    vector<string> frames = createFrames(data, frameSizes);
    cout << "\n=== " << (stuffType == 1 ? "Byte" : "Bit") << " Stuffing Results ===" << endl;
    for (size_t i = 0; i < frames.size(); i++) {
        if (stuffType == 1) {
            string stuffed = byteStuff(frames[i]);
            string destuffed = byteDestuff(stuffed);
            printByteStuffing(i + 1, frames[i], stuffed, destuffed);
        } else {
            string binary = toBinary(frames[i]);
            string stuffed = bitStuff(binary);
            string destuffed = bitDestuff(stuffed);
            printBitStuffing(i + 1, frames[i], binary, stuffed, destuffed);
        }
    }
    return 0;
}

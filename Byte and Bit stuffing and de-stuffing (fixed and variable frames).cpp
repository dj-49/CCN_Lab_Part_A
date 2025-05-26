#include <iostream>
#include <vector>
#include <string>
#include <bitset>
using namespace std;

const char FLAG = '~', ESC = '^';
const string BIT_FLAG = "01111110";

// Byte stuffing
string byteStuff(const string& data) {
    string res = string(1, FLAG);
    for (char c : data) {
        if (c == FLAG || c == ESC) res += ESC;
        res += c;
    }
    return res + FLAG;
}

// Byte destuffing
string byteDestuff(const string& data) {
    if (data.size() < 2) return "";
    string res;
    for (size_t i = 1; i < data.size() - 1; ++i) {
        if (data[i] == ESC) { ++i; if (i < data.size() - 1) res += data[i]; }
        else res += data[i];
    }
    return res;
}

// Bit stuffing
string bitStuff(const string& bin) {
    string res = BIT_FLAG;
    int ones = 0;
    for (char b : bin) {
        res += b;
        if (b == '1') { if (++ones == 5) { res += '0'; ones = 0; } }
        else ones = 0;
    }
    return res + BIT_FLAG;
}

// Bit destuffing
string bitDestuff(const string& data) {
    if (data.size() < 16 || data.substr(0, 8) != BIT_FLAG || data.substr(data.size() - 8) != BIT_FLAG) return "";
    string res, payload = data.substr(8, data.size() - 16);
    int ones = 0;
    for (size_t i = 0; i < payload.size(); ++i) {
        res += payload[i];
        if (payload[i] == '1') { if (++ones == 5 && i + 1 < payload.size() && payload[i + 1] == '0') ++i; }
        else ones = 0;
    }
    return res;
}

// ASCII to binary
string toBin(const string& s) {
    string bin;
    for (char c : s) bin += bitset<8>(c).to_string();
    return bin;
}

// Create frames
vector<string> createFrames(const string& data, const vector<int>& sizes) {
    vector<string> frames;
    size_t pos = 0;
    for (int s : sizes) {
        if (pos >= data.size()) break;
        frames.push_back(data.substr(pos, min<size_t>(s, data.size() - pos)));
        pos += s;
    }
    if (pos < data.size()) frames.push_back(data.substr(pos));
    return frames;
}

int main() {
    string data;
    int stuffType, frameType;
    cout << "Enter data: ";
    getline(cin, data);
    cout << "1. Byte Stuffing 2. Bit Stuffing: ";
    cin >> stuffType;
    cout << "1. Fixed Size 2. Variable Size: ";
    cin >> frameType;

    vector<int> sizes;
    if (frameType == 1) {
        int s;
        cout << "Frame size: ";
        cin >> s;
        sizes.assign((data.size() + s - 1) / s, s);
    } else {
        int n;
        cout << "Number of frames: ";
        cin >> n;
        sizes.resize(n);
        for (int i = 0; i < n; ++i) {
            cout << "Size for frame " << i + 1 << ": ";
            cin >> sizes[i];
        }
    }

    vector<string> frames = createFrames(data, sizes);
    cout << "\n=== " << (stuffType == 1 ? "Byte" : "Bit") << " Stuffing ===\n";
    for (size_t i = 0; i < frames.size(); ++i) {
        cout << "\nFrame " << i + 1 << ":\n";
        if (stuffType == 1) {
            string stuffed = byteStuff(frames[i]);
            string destuffed = byteDestuff(stuffed);
            cout << "Original: " << frames[i] << "\nStuffed: " << stuffed << "\nDestuffed: " << destuffed << endl;
        } else {
            string bin = toBin(frames[i]);
            string stuffed = bitStuff(bin);
            string destuffed = bitDestuff(stuffed);
            cout << "Original: " << frames[i] << "\nBinary: " << bin << "\nStuffed: " << stuffed << "\nDestuffed: " << destuffed << endl;
        }
    }
    return 0;
}

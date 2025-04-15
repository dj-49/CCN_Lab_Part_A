#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include <algorithm>

// Constants for framing
const char FLAG = '~';
const char ESC = '^';
const std::string BIT_FLAG = "01111110";

// Function prototypes
std::string byteStuff(const std::string& data);
std::string byteDestuff(const std::string& stuffedData);
std::string bitStuff(const std::string& binaryData);
std::string bitDestuff(const std::string& stuffedBits);
std::string toBinary(const std::string& data);
std::string fromBinary(const std::string& binary);
void printBinary(const std::string& binary);
std::vector<std::string> createFrames(const std::string& data, const std::vector<int>& frameSizes);

int main() {
    std::string inputData;
    int stuffingChoice, frameChoice;
    
    std::cout << "=== Data Framing with Stuffing Techniques ===" << std::endl;
    std::cout << "Enter the data to be framed: ";
    std::getline(std::cin, inputData);
    
    std::cout << "\nSelect: 1. Byte Stuffing or 2. Bit Stuffing: ";
    std::cin >> stuffingChoice;
    
    std::cout << "Select: 1. Fixed Size Frames or 2. Variable Size Frames: ";
    std::cin >> frameChoice;
    
    std::vector<int> frameSizes;
    if (frameChoice == 1) {
        int frameSize;
        std::cout << "Enter frame size: ";
        std::cin >> frameSize;
        
        // Calculate how many frames we need
        int numFrames = (inputData.length() + frameSize - 1) / frameSize;
        frameSizes.assign(numFrames, frameSize);
    } else {
        int numFrames;
        std::cout << "Enter number of frames: ";
        std::cin >> numFrames;
        
        for (int i = 0; i < numFrames; i++) {
            int size;
            std::cout << "Enter size for frame " << (i+1) << ": ";
            std::cin >> size;
            frameSizes.push_back(size);
        }
    }
    
    // Create frames
    std::vector<std::string> frames = createFrames(inputData, frameSizes);
    
    // Process based on stuffing choice
    if (stuffingChoice == 1) {  // Byte Stuffing
        std::cout << "\n=== Byte Stuffing Results ===" << std::endl;
        for (size_t i = 0; i < frames.size(); i++) {
            std::string stuffed = byteStuff(frames[i]);
            std::string destuffed = byteDestuff(stuffed);
            
            std::cout << "\nFrame " << (i+1) << ":" << std::endl;
            std::cout << "Original: " << frames[i] << std::endl;
            std::cout << "Stuffed : " << stuffed << std::endl;
            std::cout << "Destuffed: " << destuffed << std::endl;
        }
    } else {  // Bit Stuffing
        std::cout << "\n=== Bit Stuffing Results ===" << std::endl;
        for (size_t i = 0; i < frames.size(); i++) {
            std::string binaryData = toBinary(frames[i]);
            std::string stuffed = bitStuff(binaryData);
            std::string destuffed = bitDestuff(stuffed);
            
            std::cout << "\nFrame " << (i+1) << ":" << std::endl;
            std::cout << "Original: " << frames[i] << " (Binary: ";
            printBinary(binaryData);
            std::cout << ")" << std::endl;
            std::cout << "Stuffed : ";
            printBinary(stuffed);
            std::cout << "Destuffed: ";
            printBinary(destuffed);
        }
    }
    
    return 0;
}

// Create frames from input data with specified sizes
std::vector<std::string> createFrames(const std::string& data, const std::vector<int>& frameSizes) {
    std::vector<std::string> frames;
    size_t currentPos = 0;
    
    for (int size : frameSizes) {
        if (currentPos >= data.length()) break;
        
        size_t charsToTake = std::min(data.length() - currentPos, static_cast<size_t>(size));
        frames.push_back(data.substr(currentPos, charsToTake));
        currentPos += charsToTake;
    }
    
    // If there's data left, add it as a final frame
    if (currentPos < data.length()) {
        frames.push_back(data.substr(currentPos));
    }
    
    return frames;
}

// Byte stuffing: Add FLAG at start/end and escape special characters
std::string byteStuff(const std::string& data) {
    std::string stuffed(1, FLAG);  // Start with flag
    
    for (char c : data) {
        if (c == FLAG || c == ESC) stuffed += ESC;  // Add escape character if needed
        stuffed += c;  // Add the original character
    }
    
    stuffed += FLAG;  // End with flag
    return stuffed;
}

// Byte destuffing: Remove frame delimiters and handle escaped characters
std::string byteDestuff(const std::string& stuffedData) {
    if (stuffedData.size() < 2) return "";
    
    std::string destuffed;
    bool isEscaped = false;
    
    // Skip first and last FLAG characters
    for (size_t i = 1; i < stuffedData.length() - 1; i++) {
        char c = stuffedData[i];
        
        if (c == ESC && !isEscaped) {
            isEscaped = true;
        } else {
            destuffed += c;
            isEscaped = false;
        }
    }
    
    return destuffed;
}

// Bit stuffing: Add flag at start/end and insert '0' after five consecutive '1's
std::string bitStuff(const std::string& binaryData) {
    std::string stuffed = BIT_FLAG;  // Start with flag
    int consecutiveOnes = 0;
    
    for (char bit : binaryData) {
        stuffed += bit;
        
        if (bit == '1') {
            consecutiveOnes++;
            if (consecutiveOnes == 5) {
                stuffed += '0';  // Stuff a '0' after five consecutive '1's
                consecutiveOnes = 0;
            }
        } else {
            consecutiveOnes = 0;
        }
    }
    
    stuffed += BIT_FLAG;  // End with flag
    return stuffed;
}

// Bit destuffing: Remove flags and stuffed '0's
std::string bitDestuff(const std::string& stuffedBits) {
    size_t start = stuffedBits.find(BIT_FLAG);
    if (start == std::string::npos) return "";
    
    size_t end = stuffedBits.find(BIT_FLAG, start + BIT_FLAG.length());
    if (end == std::string::npos) return "";
    
    std::string data = stuffedBits.substr(start + BIT_FLAG.length(), end - start - BIT_FLAG.length());
    std::string destuffed;
    int consecutiveOnes = 0;
    
    for (size_t i = 0; i < data.length(); i++) {
        char bit = data[i];
        destuffed += bit;
        
        if (bit == '1') {
            consecutiveOnes++;
            // Skip the stuffed '0' bit that follows five consecutive '1's
            if (consecutiveOnes == 5 && i + 1 < data.length() && data[i + 1] == '0') {
                i++;  // Skip the stuffed bit
                consecutiveOnes = 0;
            }
        } else {
            consecutiveOnes = 0;
        }
    }
    
    return destuffed;
}

// Convert string to binary using std::bitset
std::string toBinary(const std::string& data) {
    std::string binary;
    for (char c : data) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}

// Convert binary string back to text
std::string fromBinary(const std::string& binary) {
    std::string result;
    for (size_t i = 0; i < binary.length(); i += 8) {
        if (i + 8 <= binary.length()) {
            std::bitset<8> bits(binary.substr(i, 8));
            result += static_cast<char>(bits.to_ulong());
        }
    }
    return result;
}

// Print binary data with spaces between bytes
void printBinary(const std::string& binary) {
    for (size_t i = 0; i < binary.length(); i++) {
        std::cout << binary[i];
        if ((i + 1) % 8 == 0) std::cout << " ";
    }
    std::cout << std::endl;
}

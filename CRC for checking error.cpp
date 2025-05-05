#include <iostream>
#include <string>
#include <bitset>
using namespace std;

// Calculate CRC for the given data and polynomial
string calcCRC(const string& data, const string& poly) {
    string rem = data + string(poly.length() - 1, '0');
    
    for (size_t i = 0; i < data.length(); i++) {
        if (rem[i] == '1') {
            for (size_t j = 0; j < poly.length(); j++) {
                rem[i+j] = (rem[i+j] == poly[j]) ? '0' : '1';
            }
        }
    }
    
    return rem.substr(data.length());
}

// Convert ASCII string to binary
string toBin(const string& s) {
    string bin;
    for (char c : s) {
        bin += bitset<8>(c).to_string();
    }
    return bin;
}

// Verify if received data contains errors
bool verifyCRC(const string& data, const string& poly) {
    string remainder = calcCRC(data.substr(0, data.length() - (poly.length() - 1)), poly);
    string expected = data.substr(data.length() - (poly.length() - 1));
    return remainder == expected;
}

int main() {
    string input, poly;
    char choice;
    
    cout << "=== CRC Calculator ===" << endl;
    cout << "Enter data: ";
    getline(cin, input);
    
    cout << "Binary input? (y/n): ";
    cin >> choice;
    cin.ignore();
    
    string data = (choice == 'y' || choice == 'Y') ? input : toBin(input);
    if (choice != 'y' && choice != 'Y')
        cout << "Binary: " << data << endl;
    
    cout << "Enter polynomial: ";
    getline(cin, poly);
    
    string crc = calcCRC(data, poly);
    string encoded = data + crc;
    
    cout << "CRC: " << crc << endl;
    cout << "Encoded data with CRC: " << encoded << endl;
    
    cout << "\nVerify received data? (y/n): ";
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        string received;
        cout << "Enter received data: ";
        cin.ignore();
        getline(cin, received);
        
        bool valid = verifyCRC(received, poly);
        cout << (valid ? "No errors detected!" : "Error detected in received data!") << endl;
    }
    
    return 0;
}

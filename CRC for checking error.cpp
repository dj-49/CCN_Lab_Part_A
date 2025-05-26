#include <iostream>
#include <string>
#include <bitset>

using namespace std;

// XOR-based CRC calculation
string calcCRC(string data, const string& poly) {
    data += string(poly.size() - 1, '0');
    for (size_t i = 0; i <= data.size() - poly.size(); ++i) {
        if (data[i] == '1') {
            for (size_t j = 0; j < poly.size(); ++j)
                data[i + j] = (data[i + j] == poly[j]) ? '0' : '1';
        }
    }
    return data.substr(data.size() - (poly.size() - 1));
}

// ASCII to binary conversion
string toBin(const string& s) {
    string bin;
    for (char c : s)
        bin += bitset<8>(c).to_string();
    return bin;
}

// Verify CRC
bool verifyCRC(const string& data, const string& poly) {
    return calcCRC(data, poly) == string(poly.size() - 1, '0');
}

int main() {
    string input, poly, data;
    char choice;

    cout << "Enter data: ";
    getline(cin, input);
    cout << "Binary input? (y/n): ";
    cin >> choice;
    cin.ignore();

    data = (choice == 'y' || choice == 'Y') ? input : toBin(input);
    if (choice != 'y' && choice != 'Y')
        cout << "Binary: " << data << endl;

    cout << "Enter polynomial: ";
    getline(cin, poly);

    string crc = calcCRC(data, poly);
    cout << "CRC: " << crc << endl;
    cout << "Encoded data: " << data + crc << endl;

    cout << "Verify data? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        cout << "Enter received data: ";
        cin.ignore();
        getline(cin, data);
        cout << (verifyCRC(data, poly) ? "No errors!" : "Errors detected!") << endl;
    }

    return 0;
}

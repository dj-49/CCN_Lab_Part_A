#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <algorithm>
using namespace std;

class SubstitutionCipher {
private:
    string plain = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-_";
    string cipher;
    unordered_map<char, char> encMap, decMap;
    
public:
    // Default constructor with random cipher
    SubstitutionCipher() : cipher(plain) {
        shuffle(cipher.begin(), cipher.end(), mt19937{random_device{}()});
        for (size_t i = 0; i < plain.length(); i++) {
            encMap[plain[i]] = cipher[i];
            decMap[cipher[i]] = plain[i];
        }
    }
    
    // Constructor with custom cipher
    SubstitutionCipher(const string& custom) {
        if (custom.length() != plain.length())
            throw invalid_argument("Custom cipher must match plain alphabet length");
        cipher = custom;
        for (size_t i = 0; i < plain.length(); i++) {
            encMap[plain[i]] = cipher[i];
            decMap[cipher[i]] = plain[i];
        }
    }
    
    // Process text using provided mapping
    string process(const string& text, const unordered_map<char, char>& map) {
        string result;
        for (char c : text)
            result += map.count(c) ? map.at(c) : c;
        return result;
    }
    
    string encrypt(const string& message) { return process(message, encMap); }
    string decrypt(const string& message) { return process(message, decMap); }
    
    void show() const {
        cout << "Plain:  " << plain << endl
             << "Cipher: " << cipher << endl;
    }
};

int main() {
    try {
        SubstitutionCipher cipher;
        string message;
        
        cout << "=== Substitution Cipher ===" << endl;
        cipher.show();
        
        cout << "\nEnter message: ";
        getline(cin, message);
        
        string encrypted = cipher.encrypt(message);
        cout << "\nEncrypted: " << encrypted << endl;
        
        cout << "Decrypted: " << cipher.decrypt(encrypted) << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}

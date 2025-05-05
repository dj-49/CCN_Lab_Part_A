#include <iostream>
#include <string>
#include <map>
#include <random>
#include <algorithm>

using namespace std;

class SubstitutionCipher {
private:
    string plainAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-_";
    string cipherAlphabet;
    map<char, char> encryptionMap, decryptionMap;

    void createMappings() {
        for (size_t i = 0; i < plainAlphabet.length(); i++) {
            encryptionMap[plainAlphabet[i]] = cipherAlphabet[i];
            decryptionMap[cipherAlphabet[i]] = plainAlphabet[i];
        }
    }

public:
    // Constructor with random cipher alphabet
    SubstitutionCipher() {
        cipherAlphabet = plainAlphabet;
        shuffle(cipherAlphabet.begin(), cipherAlphabet.end(), mt19937{random_device{}()});
        createMappings();
    }
    
    // Constructor with custom cipher alphabet
    SubstitutionCipher(const string& customCipherAlphabet) {
        if (customCipherAlphabet.length() != plainAlphabet.length())
            throw invalid_argument("Custom cipher alphabet must match plain alphabet length");
        cipherAlphabet = customCipherAlphabet;
        createMappings();
    }
    
    // Encrypt a message
    string encrypt(const string& message) {
        string result;
        for (char c : message)
            result += encryptionMap.count(c) ? encryptionMap[c] : c;
        return result;
    }
    
    // Decrypt a message
    string decrypt(const string& ciphertext) {
        string result;
        for (char c : ciphertext)
            result += decryptionMap.count(c) ? decryptionMap[c] : c;
        return result;
    }
    
    // Get the current cipher key
    string getCipherKey() const { return cipherAlphabet; }
    
    // Show the current mapping
    void displayMapping() const {
        cout << "Substitution Cipher Mapping:" << endl
             << "Plain alphabet:  " << plainAlphabet << endl
             << "Cipher alphabet: " << cipherAlphabet << endl;
    }
};

int main() {
    try {
        SubstitutionCipher cipher;
        
        cout << "=== Substitution Cipher Demonstration ===" << endl;
        cipher.displayMapping();
        
        string message;
        cout << "\nEnter a message to encrypt: ";
        getline(cin, message);
        
        string encrypted = cipher.encrypt(message);
        cout << "\nEncrypted message: " << encrypted << endl;
        
        string decrypted = cipher.decrypt(encrypted);
        cout << "Decrypted message: " << decrypted << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}

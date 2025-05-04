#include <iostream>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;

class SubstitutionCipher {
private:
    string plainAlphabet;
    string cipherAlphabet;
    map<char, char> encryptionMap;
    map<char, char> decryptionMap;

    // Creates maps for encryption and decryption
    void createMappings() {
        encryptionMap.clear();
        decryptionMap.clear();
        
        for (size_t i = 0; i < plainAlphabet.length(); i++) {
            encryptionMap[plainAlphabet[i]] = cipherAlphabet[i];
            decryptionMap[cipherAlphabet[i]] = plainAlphabet[i];
        }
    }

public:
    // Constructor with default alphabets
    SubstitutionCipher() {
        // Initialize plain alphabet (lowercase + uppercase + digits + some special chars)
        plainAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-_";
        
        // Create a randomly shuffled cipher alphabet
        cipherAlphabet = plainAlphabet;
        random_device rd;
        mt19937 g(rd());
        shuffle(cipherAlphabet.begin(), cipherAlphabet.end(), g);
        
        // Create the mapping tables
        createMappings();
    }
    
    // Constructor with custom cipher alphabet
    SubstitutionCipher(const string& customCipherAlphabet) {
        // Initialize plain alphabet
        plainAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-_";
        
        // Validate custom cipher alphabet length
        if (customCipherAlphabet.length() != plainAlphabet.length()) {
            throw invalid_argument("Custom cipher alphabet must have the same length as plain alphabet");
        }
        
        cipherAlphabet = customCipherAlphabet;
        createMappings();
    }
    
    // Encrypt a message
    string encrypt(const string& message) {
        string result;
        
        for (char c : message) {
            // If character is in our mapping, substitute it
            if (encryptionMap.find(c) != encryptionMap.end()) {
                result += encryptionMap[c];
            } else {
                // Keep characters not in our alphabet unchanged
                result += c;
            }
        }
        
        return result;
    }
    
    // Decrypt a message
    string decrypt(const string& ciphertext) {
        string result;
        
        for (char c : ciphertext) {
            // If character is in our mapping, substitute it
            if (decryptionMap.find(c) != decryptionMap.end()) {
                result += decryptionMap[c];
            } else {
                // Keep characters not in our alphabet unchanged
                result += c;
            }
        }
        
        return result;
    }
    
    // Get the current cipher key (for demonstration purposes)
    string getCipherKey() const {
        return cipherAlphabet;
    }
    
    // Show the current mapping (for demonstration purposes)
    void displayMapping() const {
        cout << "Substitution Cipher Mapping:" << endl;
        cout << "Plain alphabet:  " << plainAlphabet << endl;
        cout << "Cipher alphabet: " << cipherAlphabet << endl;
    }
};

int main() {
    try {
        // Create a cipher with random substitution
        SubstitutionCipher randomCipher;
        
        // Display information
        cout << "=== Substitution Cipher Demonstration ===" << endl;
        randomCipher.displayMapping();
        
        // Get message from user
        string message;
        cout << "\nEnter a message to encrypt: ";
        getline(cin, message);
        
        // Encrypt and display
        string encrypted = randomCipher.encrypt(message);
        cout << "\nEncrypted message: " << encrypted << endl;
        
        // Decrypt and display
        string decrypted = randomCipher.decrypt(encrypted);
        cout << "Decrypted message: " << decrypted << endl;
        
        // Demonstrate with custom key
        cout << "\n=== Custom Substitution Cipher Demonstration ===" << endl;
        
        // Example custom key (shuffled alphabet)
        string customKey = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890 .!,?-_";
        SubstitutionCipher customCipher(customKey);
        
        customCipher.displayMapping();
        
        // Encrypt and decrypt with custom cipher
        string customEncrypted = customCipher.encrypt(message);
        cout << "\nEncrypted with custom key: " << customEncrypted << endl;
        
        string customDecrypted = customCipher.decrypt(customEncrypted);
        cout << "Decrypted with custom key: " << customDecrypted << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}


#include <iostream>
#include <string>
#include <vector>

class CRC {
public:
    // Function to convert binary string to decimal
    static int binaryToDecimal(const std::string& binary) {
        int decimal = 0;
        for (char bit : binary) {
            decimal = decimal * 2 + (bit - '0');
        }
        return decimal;
    }

    // Function to convert decimal to binary string
    static std::string decimalToBinary(int decimal, int length) {
        std::string binary;
        while (decimal > 0) {
            binary = char('0' + (decimal % 2)) + binary;
            decimal /= 2;
        }
        // Pad with zeros if necessary
        while (binary.length() < length) {
            binary = '0' + binary;
        }
        return binary;
    }

    // Function to perform modulo-2 division
    static std::string mod2div(const std::string& dividend, const std::string& divisor) {
        int pick = divisor.length();
        std::string tmp = dividend.substr(0, pick);

        for (size_t i = 0; i <= dividend.length() - pick; i++) {
            if (tmp[0] == '1') {
                for (int j = 1; j < pick; j++) {
                    tmp[j - 1] = tmp[j] == divisor[j] ? '0' : '1';
                }
            } else {
                for (int j = 1; j < pick; j++) {
                    tmp[j - 1] = tmp[j];
                }
            }

            if (i < dividend.length() - pick) {
                tmp[pick - 1] = dividend[i + pick];
            }
        }

        return tmp.substr(0, pick - 1);
    }

    // Function to encode data
    static std::string encode(const std::string& data, const std::string& polynomial) {
        std::string appended_data = data;

        // Append n-1 zeros to data
        for (size_t i = 0; i < polynomial.length() - 1; i++) {
            appended_data += "0";
        }

        std::string remainder = mod2div(appended_data, polynomial);
        return data + remainder;
    }

    // Function to check if received data has any error
    static bool checkError(const std::string& received_data, const std::string& polynomial) {
        std::string remainder = mod2div(received_data, polynomial);
        return remainder.find('1') != std::string::npos;
    }
};

int main() {
    std::string data, polynomial;

    std::cout << "Enter data bits: ";
    std::getline(std::cin, data);

    std::cout << "Enter generator polynomial: ";
    std::getline(std::cin, polynomial);

    // Encode data
    std::string encoded_data = CRC::encode(data, polynomial);
    std::cout << "\nEncoded Data: " << encoded_data << std::endl;

    // Verify received data
    std::string received_data;
    std::cout << "\nEnter received data: ";
    std::getline(std::cin, received_data);

    if (CRC::checkError(received_data, polynomial)) {
        std::cout << "Error detected in received data!" << std::endl;
    } else {
        std::cout << "No error detected in received data." << std::endl;
    }

    return 0;
}

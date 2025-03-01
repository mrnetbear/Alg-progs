#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

std::string binaryToHex(const std::string& binary) {
    unsigned long long value = 0;
    for (char ch : binary) {
        if (ch == '0' || ch == '1') {
            value = (value << 1) | (ch - '0');
        }  else {
            throw std::invalid_argument("Invalid binary digit: " + std::string(1, ch));
        }
    }
    
    std::stringstream ss;
    ss << std::hex << value;
    return ss.str();
}

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {    
        std::istringstream iss(line);
        std::string binaryNumber;
        std::vector<std::string> hexNumbers;

        while (iss >> binaryNumber) {
            try {
                hexNumbers.push_back(binaryToHex(binaryNumber));
            } catch (const std::invalid_argument& e) {
                std::cerr << e.what() << '\n';
            }
        }

        for (size_t i = 0; i < hexNumbers.size(); ++i) {
            if (i > 0) {
                std::cout << " ";
            }
            std::cout << hexNumbers[i];
        }
        std::cout << std::endl;
    }

    return 0;
}
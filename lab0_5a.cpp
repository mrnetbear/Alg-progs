#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//convertation bin->hex
std::string binaryToHex(const std::string& binary) {
    unsigned long long value = 0;
    for (char ch : binary) {
        if (ch == '0' || ch == '1') {
            value = (value << 1) | (ch - '0'); //bit shifting
        }  else {
            throw std::invalid_argument("Invalid binary digit: " + std::string(1, ch)); //exception
        }
    }
    
    std::stringstream ss;
    ss << std::hex << value; // traslate number into hex
    return ss.str();
}

template<typename T>
void print_vector(const std::vector<T>& vec){
    size_t i = 0;
    for(const auto& value : vec){
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << value;
        ++i;
    }
    std::cout << std::endl;
}

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {    
        std::istringstream iss(line); //catching string stream
        std::string binaryNumber; //string for binary number
        std::vector<std::string> hexNumbers; //vector of hex numbers

        while (iss >> binaryNumber) { //translating input string elements to a binary number string
            try {
                hexNumbers.push_back(binaryToHex(binaryNumber)); //translating binaty number to hex
            } catch (const std::invalid_argument& e) { //catching incorrect symbol exception 
                std::cerr << e.what() << '\n';
            }
        }

        
        print_vector(hexNumbers);
    }

    return 0;
}
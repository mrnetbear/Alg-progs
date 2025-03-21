#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include "stack.hpp"

//Calculation of the boolean expression value

bool evaluatePrefixExpression(const std::string &expression, Stack<std::string> &stack ){
    std::istringstream iss(expression);
    std::string token;

    std::list<std::string> tokens;
    while (iss >> token) {
        tokens.push_front(token);
    }
    for (const auto &t : tokens){
        if (t == "0" || t == "1" )
            stack.push(t);
        else if (t == "!" || t == "&&" || t == "||"){
            if (stack.empty())
                throw std::invalid_argument("Not enough operands");
            if (t == "!"){
                std::string operand = stack.top();
                stack.pop();
                stack.push(operand == "1" ? "0" : "1");
            }
            else{
                if (stack.empty()) 
                    throw std::invalid_argument("Invalid expression: not enough operands for operator " + t);
                std::string operand1 = stack.top();
                stack.pop();
                std::string operand2 = stack.top();
                stack.pop(); 

                bool    value1 = (operand1 == "1"),
                        value2 = (operand2 == "1");
                if (t == "&&")
                    stack.push(value1 && value2? "1" : "0");
                else if (t == "||")
                    stack.push(value1 || value2? "1" : "0");
            }
        }
        else
            throw std::invalid_argument("Invalid token: " + t);
    }

    if (stack.empty())
        throw std::invalid_argument("Invalid expression: no result");
    
    std::string result = stack.top();
    stack.pop();
    if (!stack.empty())
        throw std::invalid_argument("Invalid expression: too many operands");
    
    return result == "1";
}

//input function
void processInput(Stack<std::string>& stack) {
    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            bool result = evaluatePrefixExpression(line, stack);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main() {
    #ifdef USE_LIST_STACK
    ListStack<std::string> stack;
    #else
    VectorStack<std::string> stack;
    #endif

    processInput(stack);

    return 0;
}

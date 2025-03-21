#include "stack.hpp"

//VectorStack methods realization

template <typename T>
void VectorStack<T>::push(const T& value) {
    data.push_back(value); //push element to stack
}

template <typename T>
void VectorStack<T>::pop() {
    if (data.empty()) {
        throw std::out_of_range("Stack is empty"); //check if stack is empty
    }
    data.pop_back(); //pop element from stack
}

template <typename T>
const T& VectorStack<T>::top() {
    if (data.empty()) {
        throw std::out_of_range("Stack is empty"); //check if stack is empty
    }
    return data.back(); //return top element in stack
}

template <typename T>
bool VectorStack<T>::empty() const {
    return data.empty(); //check if stack is empty
}

//ListStack methods realization

template <typename T>
void ListStack<T>::push(const T& value) {
    data.push_back(value); //push element to stack
}

template <typename T>
void ListStack<T>::pop() {
    if (data.empty()) {
        throw std::out_of_range("Stack is empty"); //check if stack is empty
    }
    data.pop_back(); //pop element from stack
}

template <typename T>
const T& ListStack<T>::top() {
    if (data.empty()) {
        throw std::out_of_range("Stack is empty"); //check if stack is empty
    }
    return data.back(); //return top element in stack
}

template <typename T>
bool ListStack<T>::empty() const {
    return data.empty(); //check if stack is empty
}

template class VectorStack<std::string>;
template class ListStack<std::string>;
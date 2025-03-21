#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <vector>
#include <string>
#include <list>

//define stack 

template <typename T> 
class Stack {
public: 
    virtual ~Stack() {}
    virtual void push(const T& value) = 0; //push element into stack
    virtual void pop() = 0; //pop element from stack
    virtual const T& top() = 0; //top element in stack
    virtual bool empty() const = 0; //check if stack is empty
};

// define stack implementation using vector

template <typename T>
class VectorStack : public Stack<T> {
private:
    std::vector<T> data; // stack data vector
public:
    void push(const T& value) override; // push element into stack
    void pop() override; // pop element from stack
    const T& top() override; // top element in stack
    bool empty() const override; // check if stack is empty
};

// define stack implementation using list

template <typename T>
class ListStack : public Stack<T> {
private:
    std::list<T> data; // stack data list
public:
    void push(const T& value) override; // push element into stack
    void pop() override; // pop element from stack
    const T& top() override; // top element in stack
    bool empty() const override; // check if stack is empty
};

#endif // STACK_H
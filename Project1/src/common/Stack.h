#pragma once
#include <cstdlib>
#include <iostream>


template <typename T>
class Stack {
private:
    static const int MaxSize = 100; // Maximum size of the stack
    T data[MaxSize]; // Maximum size of the stack
    int top; // Index of the top element
public:
    Stack() : top(-1) {} // Constructor initializes top to -1
    
    Stack(const Stack& s) : top(s.top) { // Copy constructor
        for (int i = 0; i <= top; ++i) {
            data[i] = s.data[i];
        }
    }
    
    ~Stack() { top = -1; } // Destructor resets top

    bool isEmpty() const { return top == -1; } // Check if stack is empty

    bool isFull() const { return top == MaxSize; } // Check if stack is full

    void Push(T x) {
        if (isFull()) {
            std::cerr << "Stzck is full." << std::endl;
            return;
        }
        data[++top] = x;
    }

    T Top() const {
        if (isEmpty()) {
            std::cerr << "Stack is empty." << std::endl;
            exit(EXIT_FAILURE);
        }
        T x = data[top];
        return x;
    }

    void Pop() {
        if (isEmpty()) {
            std::cerr << "Stack is empty." << std::endl;
            return;
        }
        --top;
    }

    int Length() const { return top + 1; } // Get the number of elements in the stack

};

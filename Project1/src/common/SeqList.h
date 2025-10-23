#pragma once
#include <cstdlib>
#include <iostream>


template <typename T>
class SeqList {
private:
    static const int MaxSize = 100; // Maximum size of the list
    T data[MaxSize];
    u_int32_t length;
public:
    SeqList() {length = 0;}

    SeqList(T arr[], int n) {
        if (n > MaxSize) {
            std::cerr << "Array size exceeds maximum limit." << std::endl;
            length = 0;
            return;
        }
        for (int i = 0; i < n; ++i) {
            data[i] = arr[i];
        }
        length = n;
    }

    ~SeqList() {
        length = 0;
    }

    int Length() const {
        return length;
    }

    T Get(int i) const {
        if (i < 0 || i >= length) {
            std::cerr << "Index out of bounds." << std::endl;
            exit(EXIT_FAILURE);
        }
        return data[i];
    }

    void Insert(int i, T x) {
        if (length >= MaxSize) {
            std::cerr << "List is full." << std::endl;
            return;
        }
        if (i < 0 || i > length) {
            std::cerr << "Index out of bounds." << std::endl;
            return;
        }
        for (int j = length; j > i; --j) {
            data[j] = data[j - 1];
        }
        data[i] = x;
        ++length;
    }

    T Delete(int i) {
        if (i < 0 || i >= length) {
            std::cerr << "Index out of bounds." << std::endl;
            exit(EXIT_FAILURE);
        }
        T deletedValue = data[i];
        for (int j = i; j < length - 1; ++j) {
            data[j] = data[j + 1];
        }
        --length;
        return deletedValue;
    }

    void Push_back(T x) {
        Insert(length, x);
    }

    void Print() const {
        for (int i = 0; i < length; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    void Clear() {
        length = 0;
    }
    
};




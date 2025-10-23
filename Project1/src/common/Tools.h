#pragma once
#include <cstdlib>
#include <iostream>
typedef std::pair<double, int> PDI;
template <typename T>

void Swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
T partion(T arr[], int low, int high) {
    T pivot = arr[low];
    int i = low, j = high;
    while (i < j) {
        while (i < j && arr[j] >= pivot) --j;
        if (i < j) arr[i++] = arr[j];
        while (i < j && arr[i] <= pivot) ++i;
        if (i < j) arr[j--] = arr[i];
    }
    arr[i] = pivot;
    return i;
}

template <typename T>
void QuickSort(T arr[], int low, int high) {
    if (low < high) {
        int pivot = partion(arr, low, high);
        QuickSort(arr, low, pivot - 1);
        QuickSort(arr, pivot + 1, high);
    }
}

// std::string -> double
double ToDouble(const std::string& str) {
    try {
        return std::stod(str);  // string to double
    } catch (const std::exception& e) {
        std::cout << "转换失败: " << str << std::endl;
        return 0.0;
    }
}

PDI Feri(double x, int y) {
    if (y == 1) return {x, 1};
    if (y == 0) return {0, 0};
    return {x * y, y - 1};
}

int ToInt(char c) {
    return c - '0';
}
#pragma once
#include <iostream>

using namespace std;

const int initCapacity = 8;

class Vector {
private:
    double* data;
    int capacity; 
    int size;     

    // Метод для увеличения емкости массива
    void expand(int newCapacity) {
        if (newCapacity <= capacity)
            return;
        double* newData = new double[newCapacity];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector(int initialCapacity = initCapacity) : capacity(initialCapacity), size(0) {
        data = new double[capacity];
    }

    ~Vector() {
        delete[] data;
    }


    // методы Vector
    void push(double value) {
        if (size == capacity) {
            expand(capacity * 2); // удвоение емкости при заполнении
        }
        data[size++] = value;
    }

    void resize(int newSize) {
        if (newSize > capacity) {
            expand(newSize);
        }
        size = newSize;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    double& operator[](int index) const {
        return data[index];
    }

    void clear() {
        delete[] data;
        capacity = initCapacity;
        size = 0;
        data = new double[capacity];
    }

    void print_Vector() {
        for (int i = 0; i < size; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};
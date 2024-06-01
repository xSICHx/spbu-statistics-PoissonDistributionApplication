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
        for (int i = size; i < newCapacity; ++i) {
            newData[i] = 0;
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    Vector(int initialCapacity = initCapacity) : capacity(initialCapacity), size(0) {
        data = new double[capacity];
        for (int i = size; i < capacity; ++i) {
            data[i] = 0;
        }
    }

    Vector(const Vector& other) : data(new double[other.capacity]), size(other.size), capacity(other.capacity) {
        std::copy(other.data, other.data + other.capacity, data);
    }

    Vector(Vector&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity){
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    ~Vector() {
        delete[] data;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new double[capacity];
        std::copy(other.data, other.data + other.capacity, data);
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    double& operator[](int index) const {
        return data[index];
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

    void reserve(int newCapacity) {
        expand(newCapacity);
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    double* getData() {
        double* tmp = new double[size];
        copy(data, data + size, tmp);
        return tmp;
    }

    

    void clear() {
        delete[] data;
        capacity = initCapacity;
        size = 0;
        data = new double[capacity];
        for (int i = 0; i < capacity; ++i) {
            data[i] = 0;
        }
    }

    void print_Vector() {
        for (int i = 0; i < size; ++i) {
            cout << data[i] << " ";
        }
        cout << endl;
    }


    
};
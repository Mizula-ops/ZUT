#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <string>

template<typename T>
class DynamicArray {
    int capacity;
    int size;
    T* array;

    void resize() {
        capacity *= 2;
        T* newArray = new T[capacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }
    
public:
    DynamicArray(int initialCapacity = 16) : capacity(initialCapacity), size(0) {
        array = new T[capacity];
    }

    ~DynamicArray() {
        delete[] array;
    }

    void push_back(const T& value) {
        if (size == capacity) {
            resize();
        }
        array[size++] = value;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return array[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return array[index];
    }

    int getSize() const { return size; }

    int getCapacity() const { return capacity; }

    void clear() {
        size = 0;
    }

    void print() const {
        std::cout << "Capacity: " << capacity << ", Size: " << size << "\nElements: ";
        for (int i = 0; i < size; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }
};

#endif

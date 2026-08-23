#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <string>

template<typename T>
struct ElementList {
    ElementList* next;
    ElementList* prev;
    T data;

    ElementList(T data) : next(nullptr), prev(nullptr), data(data) {}
};

template<typename T>
class LinkedList {
    ElementList<T>* head;
    ElementList<T>* tail;
    unsigned int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() { clear(); }

    void frontpush(T value) {
        ElementList<T>* newNode = new ElementList<T>(value);

        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }
    ElementList<T>* begin() {
        return head;
    }

    // Funkcja implementująca end
    ElementList<T>* end() {
        ElementList<T>* temp = head;
        while (temp && temp->next) {
            temp = temp->next;
        }
        return temp;
    }
    void backpush(T value) {
        ElementList<T>* newNode = new ElementList<T>(value);

        if (!tail) {
            head = tail = newNode;
        }
        else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void backpop() {
        if (!tail) {
            std::cout << "List is empty\n";
            return;
        }

        ElementList<T>* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        size--;
    }

    void frontpop() {
        if (!head) {
            std::cout << "List is empty\n";
            return;
        }

        ElementList<T>* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        size--;
    }

    T search(unsigned int index) const {
        if (index >= size) {
            std::cout << "Index out of bounds\n";
            return T();
        }

        ElementList<T>* current;
        if (index > size / 2) {
            current = tail;
            for (int i = size - 1; i > index; --i) {
                current = current->prev;
            }
        }
        else {
            current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
        }
        return current->data;
    }

    void clear() {
        while (head) {
            ElementList<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    bool empty() const { return size == 0; }

    unsigned int getSize() const { return size; }

    std::string print() const {
        std::string result = "List size: " + std::to_string(size) + "\nElements: ";
        ElementList<T>* current = head;
        while (current) {
            result += std::to_string(current->data) + " ";
            current = current->next;
        }
        return result;
    }
   
};

#endif

#include <iostream>
#include <string>
#include <ctime>
#ifndef LINKED_LIST_H
#define LINKED_LIST_H 
template<typename T>
struct ElementList {
    ElementList* next, * prev;
    T data;
public:
    ElementList(T data) :next(nullptr), prev(nullptr), data(data) {}
};
template<typename T>
class LinkedList {
    ElementList<T>* head, * tail;
    unsigned int size;
public:
    LinkedList() :head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() { clear(); }
    void frontpush(T value)
    {
        ElementList<T>* p = new ElementList<T>(value);

        if (!head)  head = tail = p;
        else
        {

            p->next = head;
            head->prev = p;
            head = p;
        }
        size++;
    }
    int get_size() {
        return size;
    }
    void backpush(T value)
    {
        ElementList<T>* p = new ElementList<T>(value);
        if (!head)  head = tail = p;
        else
        {
            p->prev = tail;
            tail->next = p;
            tail = p;
        }
        size++;
    }
    void backpop()
    {
        ElementList<T>* p = tail;

        if (!head) { std::cout << "Lista jest pusta" << std::endl; return; }
        else if (head == tail) head = tail = nullptr;
        else
        {
            tail = p->prev;
            delete p;
            tail->next = nullptr;
        }
        size--;

    }
    void frontpop()
    {
        ElementList<T>* p = head;

        if (!head) { std::cout << "Lista jest pusta" << std::endl; return; }
        else if (head == tail) head = tail = nullptr;
        else
        {
            head = p->next;
            head->prev = nullptr;
            delete p;

        }
        size--;
    }
    int index(unsigned int index) {
        ElementList<T>* p;
        if (index > size - 1) { std::cout << "Index nie istnieje" << std::endl; return 0; }
        if (index > size / 2)
        {
            p = tail;
            for (int i = size - 1;i > index;i--)
                p = p->prev;
            return p->data;
        }
        else
        {
           // cout << " Index istnieje";
            p = head;
            for (int i = 0;i < index;i++)
                p = p->next;
            return p->data;
        }
    }
    T search(unsigned int index) {
        ElementList<T>* p;
        if (index > size - 1) { std::cout << "Index nie istnieje" << std::endl; return 0; }
        if (index > size / 2)
        {
            p = tail;
            for (int i = size - 1;i > index;i--)
                p = p->prev;
            return p->data;
        }
        else
        {
            p = head;
            for (int i = 0;i < index;i++)
                p = p->next;
            return p->data;
        }
    }
    void change(unsigned int index, T value) {
        ElementList<T>* p;
        if (index > size - 1) { std::cout << "Index nie istnieje" << std::endl; return; }
        if (index > size / 2)
        {
            p = tail;
            for (int i = size - 1;i > index;i--)
                p = p->prev;
            p->data = value;
        }
        else
        {
            p = head;
            for (int i = 0;i < index;i++)
                p = p->next;
            p->data = value;
        }
    }
    ElementList<T>* found(T key, int(*cmp)(T*, T*))
    {
        ElementList<T>* p = head;
        while (p)
        {
            if (cmp(p->data, key) == 0)
            {
                return p;
            }
            p = p->next;
        }
        return nullptr;
    }
    bool find_and_remove(T key, int(*cmp)(T*, T*))
    {
        ElementList<T>* p = found(key, cmp);
        if (p == nullptr) return false;

        if (p == head)frontpop();
        else if (p == tail)backpop();
        else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
        }
        size--;
        return true;
    }
    std::string print() {
        ElementList<T>* p = head;
        std::string krokiet = "Lista o wartosci: " + std::to_string(size) + "\n Elementy: ";
        if (size > 25)
        {
            for (int i = 0;i < 5;i++)
            {
                krokiet += std::to_string(p->data) + " ";
                p = p->next;
            }
            krokiet += " ...  ";
            p = tail;
            for (int i = 0;i < 4;i++) p = p->prev;
            while (p)
            {
                krokiet += std::to_string(p->data) + " ";
                p = p->next;
            }
        }
        else
        {
            while (p)
            {
                krokiet += std::to_string(p->data) + " ";
                p = p->next;
            }
        }
        return krokiet;
    }
    std::string print(string(*see)(T)) {
        ElementList<T>* p = head;
        std::string krokiet = "Lista o wartosci: " + std::to_string(size) + "\n Elementy: ";

        if (size > 30)
        {
            for (int i = 0;i < 5;i++)
            {
                krokiet += see(p->data) + " ";
                p = p->next;
            }
            krokiet += " ...  ";
            p = tail;
            for (int i = 0;i < 4;i++) p = p->prev;
            while (p)
            {
                krokiet += see(p->data) + " ";
                p = p->next;
            }
        }
        else
        {
            while (p)
            {
                krokiet += see(p->data) + " ";
                p = p->next;
            }
        }
        return krokiet;
    }
    
    void clear()
    {
        ElementList<T>* p = head;
        while (p)
        {
            p = p->next;
            delete head;
            head = p;
        }
        size = 0;
    }

};



#endif



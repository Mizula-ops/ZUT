#ifndef LISTA_H
#define LISTA_H
#include <iostream>
#include <string>
#include <ctime>

template<typename T>
struct ElementList {
    ElementList* next, *prev;
    T data;
public:
    ElementList(T data) :next(nullptr), prev(nullptr), data(data) {}
};
template<typename T>
class LinkedList {
    ElementList<T>* head, *tail;
    unsigned int size;
public:
    LinkedList():head(nullptr),tail(nullptr),size(0){}
    ~LinkedList() {clear();}
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
    void backpush(T value)
    {
        ElementList<T>* p = new ElementList<T>(value);
        if (!head)  head=tail = p; 
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
     T headReturn()
    {
        ElementList<T>* p = head;
        return p->data;
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
    void change(unsigned int index,T value) {
        ElementList<T>* p;
        if (index > size - 1) { std::cout << "Index nie istnieje" << std::endl; return ; }
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
            p->data=value;
        }
    }
    
   
    std::string print() {
        ElementList<T>* p = head;
        std::string krokiet = "Lista o wartosci: "+std::to_string(size) + "\n Elementy: ";
        if (size > 10)
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
    bool isNoEmpty(){
        if (size==0) return false;
        else return true;
    };

};
#endif






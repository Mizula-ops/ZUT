//ALGO2 IS1 211B LAB01
//Artur Mizuła
//ma55831@zut.edu.pl
#include <iostream>
#include <string>
#include <ctime>
struct some_object
{
    int field_1;
    char field_2;
};
std::string object_string(some_object* so1)
{
    return "|" + std::to_string(so1->field_1) + so1->field_2 + "|";
}
some_object* generate_some_objects()
{
    some_object* so = new some_object();
    so->field_1 = rand() % 10000;
    so->field_2 = 'a' + rand() % 26;
    return so;
}
int some_objects_cmp(some_object* so1, some_object* so2)
{
    int diff = so1->field_1 - so2->field_1;
    if (diff != 0)
        return diff;
    return so1->field_2 - so2->field_2;
}
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
    ElementList<T>* found(T key, int(*cmp)(some_object*, some_object*))
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
    bool find_and_remove(T key, int(*cmp)(some_object*, some_object*))   
    {
        ElementList<T>* p = found(key,cmp);
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
    std::string print(std::string(*objects_string)(some_object*)) {
        ElementList<T>* p=head;
        std::string krokiet = "Lista o wartosci: " + std::to_string(size) + "\n Elementy: ";
        
        if (size > 10)
        {
            for (int i = 0;i < 5;i++)
            {
                krokiet += objects_string(p->data) + " ";
                p = p->next;
            }
            krokiet += " ...  ";
            p = tail;
            for (int i = 0;i < 4;i++) p = p->prev;
            while (p)
            {
                krokiet += objects_string(p->data) + " ";
                p = p->next;
            }
        }
        else
        {
            while (p)
            {
                krokiet += objects_string(p->data) + " ";
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


int main1()
{
    srand(static_cast<unsigned int>(time(0)));
    const int MAX_ORDER = 6; 
    LinkedList < some_object* >* wiadukt = new LinkedList < some_object* >(); 
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);
        clock_t t1 = clock();
        for (int i = 0; i < n; i++) {
            some_object* so = generate_some_objects();
            wiadukt->frontpush(so);
        }

        std::cout<<wiadukt->print(object_string)<<std::endl;
        some_object* so = generate_some_objects();
       
        
        clock_t t2 = clock();
        double time = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Dodano: " << n << " Elementow. Czas to: " << time << " sekundy" << std::endl;
        
        const int m = pow(10, 4); 
        t1 = clock();
        for (int i = 0; i < m; i++) {
            some_object* so = generate_some_objects();
            
            
                 wiadukt->find_and_remove(so,some_objects_cmp);
            delete so;
        }
        t2 = clock();
        double time1 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
        std::cout << "Porownano: " << m << " Elementow. Czas to: " << time1 << " sekundy" << std::endl;
        std::cout << wiadukt->print(object_string) << std::endl;
        wiadukt->clear();
    }
    delete wiadukt;
    return 0;
}




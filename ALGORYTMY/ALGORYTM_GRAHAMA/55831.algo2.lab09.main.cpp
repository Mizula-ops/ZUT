// ALGO2 IS1 211B LAB09
// ARTUR MIZUŁA
// ma55831@zut.edu.pl


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include "heap0.h"
#include "dynamicArray.h"
#include "linked_list.h"
using namespace std;
struct some_object
{
    double x;
    double y;
    int index;
};
double iloczyn_wektorowy(some_object* origin, some_object* punkt_a, some_object* punkt_b)
{
    return (punkt_b->x - punkt_a->x) * (origin->y - punkt_a->y) - (punkt_b->y - punkt_a->y) * (origin->x - punkt_a->x);
}
double distance(some_object* punkt_a, some_object* punkt_b)
{
    return (punkt_a->x - punkt_b->x) * (punkt_a->x - punkt_b->x) + (punkt_a->y - punkt_b->y) * (punkt_a->y - punkt_b->y);
}
bool compare(some_object* a, some_object* b, some_object* origin)
{
    double iw= iloczyn_wektorowy(origin, a, b);
    if (iw == 0) {
        return distance(origin, a) < distance(origin, b);
    }
    return iw > 0;
}
double some_objects_cmp1(some_object* so1, some_object* so2)
{
    double diff = so1->y - so2->y;
    if (diff != 0)
        return diff;
    return so1->x - so2->y;
}

double give_some_objects(some_object* so1, int which)
{
    if (which == 1) {
        return so1->x;
    }
    else if (which == 2) {
        return so1->y;
    }
   
}
std::string see(some_object* so1)
{
    return std::to_string(so1->x) + " " + to_string(so1->y) +   "|";
}
std::string see_index(some_object* so1)
{
    return std::to_string(so1->index) +"|";
}

void load_points_from_file(int i, dynamic_array<some_object*>* da, int& n) {
    fstream uchwyt;
    string linia;
    string file_name = "points" + to_string(i) + ".txt";
    uchwyt.open(file_name);

    if (!uchwyt.is_open()) {
        return;
    }

    if (getline(uchwyt, linia)) {
        n = stoi(linia); 
    }

    for (int j = 0; j < n; j++) { 
        some_object* so = new some_object();

        if (getline(uchwyt, linia)) {
            stringstream ss(linia);
            ss >> so->x >> so->y;
            so->index = j; 
            da->backpush(so); 
        }
    }

    uchwyt.close();
}
template<typename T>
void graham(LinkedList<int>* ll, dynamic_array<T>* da, int n, binary_heap<T>* bh) {
    ll->clear();
    da->clear();

    ll->backpush(0);
    ll->backpush(1);
    da->backpush(bh->give_array(0));
    da->backpush(bh->give_array(1));

    for (int i = 2; i <= n; i++) {
        while (ll->get_size() > 1) {
            auto second_last_point = bh->give_array(ll->index(ll->get_size() - 2));
            auto last_point = bh->give_array(ll->index(ll->get_size() - 1));
            auto current_point = bh->give_array(i);

            if (iloczyn_wektorowy(second_last_point, last_point, current_point) >= 0) {
                ll->backpop();
                da->back_pop();
            }
            else {
                break;
            }
        }

        ll->backpush(i);
        da->backpush(bh->give_array(i));
    }
}
//cout << da->print_da(see) << endl;
 //cout << bh->print_h(see) << endl;
int main() {
    for (int i = 1;i < 6;i++)
    {
        int n =0;
        dynamic_array<some_object*>* da = new dynamic_array <some_object*> ();
        load_points_from_file(i, da, n);
        
        

        da->set_min(some_objects_cmp1);
        some_object* so = da->index(0);
        da->front_pop();
        some_object** dynamic_array = da->return_Array();
        
        clock_t t1 = clock();
        binary_heap < some_object* >* bh = new binary_heap < some_object* >(dynamic_array, n,so, compare, true);
        bh->sort(so,compare);
        bh->frontPush(so);
        clock_t t2 = clock();
        
        double time_sort = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
       cout << "Posortowano " << n<< " punktow. Uzyto Heap Sort. Czas to: " << time_sort << " sekund" << endl << endl;
       LinkedList<int>* ll = new LinkedList<int>();
        
       da->clear();
        graham(ll,da,n, bh);
        t2 = clock();

        double time_graham = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
        cout<<da->print_da(see_index)<<endl;
        cout << "Stworzono strukture Grahama Czas to: " << time_graham << " sekund" << endl << endl;
    }
    return 0;
   
} 
/////////////////////////////
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H 
#include <iostream>
#include<string>
#include <ctime>
#include <cmath>

template<typename T>
class dynamic_array
{

	int maks;
	int rozmiar;
	T* array;
public:
	dynamic_array() {
		maks = 200;
		rozmiar = 0;
		array = new T[maks];
	};
	~dynamic_array() {
		clear();
	}
	void backpush(T data)
	{
		if (rozmiar < maks)
		{
			array[rozmiar] = data;
			rozmiar++;
		}
		else
		{
			maks *= 2;
			T* newArray = new T[maks];
			for (int i = 0;i <= rozmiar;i++) {
				newArray[i] = array[i];

			}
			newArray[rozmiar] = data;
			rozmiar++;
			delete[] array;
			array = newArray;
		}

	}


	void set_min(double(*cmp)(T, T))
	{
		T tmp = array[0];
		int minIndex = 0;
		for (int i = 1;i < rozmiar;i++)
		{
			if (cmp(tmp, array[i]) > 0)
			{
				tmp = array[i];
				minIndex = i;
			}
		}
		if (minIndex != 0)
			swap(array[0], array[minIndex]);

	}
	void front_pop()
	{

		for (int i = 0; i < rozmiar - 1; i++)
		{
			array[i] = array[i + 1];
		}

		rozmiar--;
	}
	void back_pop()
	{
		array[rozmiar] = NULL;
		rozmiar--;
	}
	T index(int which)
	{
		if (which > maks) return 0;
		else if (which > rozmiar - 1) return 0;
		else return array[which];
	}



	T* return_Array()
	{
		return array;
	}

	int get_size()
	{
		return rozmiar;
	}
	double suma(double(*give)(T, int))
	{
		double wynik = 0;
		for (int i = 0;i < rozmiar;i++)
		{
			wynik = wynik + give_some_objects(array[i], 3);
		}
		return wynik;
	}


	std::string print_da(std::string(*see)(T))
	{


		std::string krokiet = "Elementy:  |";

		if (rozmiar > 30)
		{
			for (int i = 0;i < 5;i++) krokiet += see(array[i]);
			krokiet += "...|";
			for (int i = rozmiar - 5;i < rozmiar;i++) krokiet += see(array[i]);
		}
		else
		{
			for (int i = 0;i < rozmiar;i++) krokiet += see(array[i]);
		}
		return krokiet;

	}
	void clear()
	{
		for (int i = 0;i < rozmiar;i++) {
			array[i] = NULL;
		}
		rozmiar = 0;
		maks = 200;
	}



};




#endif
/////////////////////////////

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



/////////////////
#ifndef HEAP_H
#define HEAP_H

using namespace std;
#include <iostream>
#include<string>
#include <ctime>
#include <cmath>

template<typename T>
class binary_heap
{

	int rozmiar;
	int maks = 2;
	T* array;

public:
	binary_heap() {
		rozmiar = 0;
		array = new T[maks];
	};
	~binary_heap() {
		clear();
	}

	binary_heap(T* existingArray, int n, bool fixed)
	{
		array = existingArray;
		rozmiar = n;
		maks = n;

		if (fixed) heapifyBottomDown();
		else heapifyTopDown();

	}
	binary_heap(T* existingArray, int n, T origin, bool(*cmp)(T, T, T), bool fixed)
	{
		array = existingArray;
		rozmiar = n;
		maks = n;

		if (fixed) heapifyBottomDown(origin, cmp);
		else heapifyTopDown(origin, cmp);

	}

	void heapifyTopDown(T origin, bool(*cmp)(T, T, T))
	{
		for (int i = 0;i < rozmiar;i++)
			heapifyUp(i, origin, cmp);
	}
	void heapifyBottomDown(T origin, bool(*cmp)(T, T, T))
	{
		for (int i = (rozmiar - 2) / 2;i >= 0;i--)
			heapifyDown(i, origin, cmp);
	}

	void sort(T origin, bool (*cmp)(T, T, T))
	{
		int nasz_rozmiar = rozmiar;
		while (rozmiar > 1) {
			swap(array[0], array[rozmiar - 1]);
			rozmiar--;
			heapifyDown(0, origin, cmp);
		}
		rozmiar = nasz_rozmiar;
	}
	void add(T data)
	{
		if (rozmiar < maks)
		{
			array[rozmiar] = data;

		}
		else
		{
			maks *= 2;
			T* newArray = new T[maks];
			for (int i = 0;i <= rozmiar;i++) {
				newArray[i] = array[i];

			}
			newArray[rozmiar] = data;

			delete[] array;
			array = newArray;
		}

		heapifyUp(rozmiar);
		rozmiar++;
	}
	template<typename T>
	void frontPush(T data) {

		if (rozmiar >= maks) {
			maks *= 2;
			T* newArray = new T[maks];


			for (int i = 0; i < rozmiar; i++) {
				newArray[i + 1] = array[i];
			}


			newArray[0] = data;


			delete[] array;
			array = newArray;
		}
		else {

			for (int i = rozmiar; i > 0; --i) {
				array[i] = array[i - 1];
			}
			array[0] = data;
		}


		rozmiar++;
	}

	void heapifyUp(int index) {
		if (index == 0)
			return;

		int parent = (index - 1) / 2;

		if (array[index] > array[parent]) {

			T temp = array[index];
			array[index] = array[parent];
			array[parent] = temp;
			heapifyUp(parent);
		}

	}
	void del_root()
	{

		array[0] = array[rozmiar - 1];
		rozmiar--;
		heapifyDown(0);

	}
	void heapifyDown(int index) {
		if (index == rozmiar)
			return;
		int leftchild = 2 * index + 1;
		int rightchild = 2 * index + 2;
		int largest = index;
		if (leftchild < rozmiar && array[leftchild] > array[largest])
			largest = leftchild;
		if (rightchild < rozmiar && array[rightchild] > array[largest])
			largest = rightchild;

		if (largest != index) {
			swap(array[index], array[largest]);
			heapifyDown(largest);
		}

	}
	void add(T data, double(*cmp)(T, T))
	{
		if (rozmiar < maks)
		{
			array[rozmiar] = data;

		}
		else
		{
			maks *= 2;
			T* newArray = new T[maks];
			for (int i = 0;i <= rozmiar;i++) {
				newArray[i] = array[i];

			}
			newArray[rozmiar] = data;

			delete[] array;
			array = newArray;
		}

		heapifyUp(rozmiar, cmp);
		rozmiar++;
	}

	void heapifyUp(int index, T origin, bool(*cmp)(T, T, T)) {
		if (index == 0)
			return;

		int parent = (index - 1) / 2;

		if (cmp(array[index], array[parent], origin)) {

			T temp = array[index];
			array[index] = array[parent];
			array[parent] = temp;
			heapifyUp(parent, origin, cmp);
		}

	}
	T del_root1(double(*cmp)(T, T, T))
	{
		T root = array[0];
		array[0] = array[rozmiar - 1];
		rozmiar--;
		heapifyDown(0, cmp);
		return root;
	}
	void heapifyDown(int index, T origin, bool(*cmp)(T, T, T)) {
		if (index >= rozmiar) return;
		int leftchild = 2 * index + 1;
		int rightchild = 2 * index + 2;
		int largest = index;
		if (leftchild < rozmiar && cmp(array[leftchild], array[largest], origin))
			largest = leftchild;
		if (rightchild < rozmiar && cmp(array[rightchild], array[largest], origin))
			largest = rightchild;

		if (largest != index) {
			swap(array[index], array[largest]);
			heapifyDown(largest, origin, cmp);
		}
	}
	double give_some_array(int i, int which, double(*give)(T, int))
	{
		return give(array[i], which);
	}
	T give_array(int i)
	{
		return array[i];
	}
	T* return_heap()
	{
		return array;
	}

	std::string print_h(string(*see)(T))
	{
		std::string krokiet = "Elementy:  |";

		if (rozmiar > 10)
		{
			for (int i = 0;i < 5;i++) krokiet += see(array[i]);
			krokiet += "...|";
			for (int i = rozmiar - 5;i < rozmiar;i++) krokiet += see(array[i]);
		}
		else
		{
			for (int i = 0;i < rozmiar;i++) krokiet += see(array[i]);
		}
		return krokiet;
	}
	void clear()
	{
		for (int i = 0;i <= rozmiar;i++) {
			array[i] = NULL;
		}
		rozmiar = 0;
		maks = 2000;
	}



};





#endif
/////////////////

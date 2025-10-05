//ALGO2 IS1 211B LAB07
//Artur Mizuła
//ma55831@zut.edu.pl

#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "55831.algo2.lab05.main.h"
using namespace std;
struct some_object
{
	double field_1;
	char field_2;
};
double some_objects_key_double(some_object* so1)
{
	double key = so1->field_1;
	return key;
}
std::string see(some_object* so1)
{
	return std::to_string(so1->field_1) + "" + so1->field_2 + "|";
}
double some_objects_cmp(some_object* so1, some_object* so2)
{

	double diff = so1->field_1 - so2->field_1;
	if (diff != 0)
		return diff;
	return so1->field_2 - so2->field_2;
}
using namespace std;
void counting_sort(int* array,int n,int m)
{
	vector<int> count(m,0);
	for (int i = 0;i < n;i++)
		count[array[i]]++;
	int index = 0;
	for (int i = 0; i < m; i++)
	{
		while (count[i] > 0)
		{
			array[index++] = i;
			count[i]--;
		}
	}
}

void insertionSort(vector<int>& bucket) {
	for (size_t i = 1; i < bucket.size(); i++) {
		int temp = bucket[i];
		int j = i - 1;


		while (j >= 0 && bucket[j] > temp) {
			bucket[j + 1] = bucket[j];
			j--;
		}
		bucket[j + 1] = temp;
	}
}
template<typename T>
void insertionSort(vector<T>& bucket, double(*cmp)(T, T)) {
	for (size_t i = 1; i < bucket.size(); i++) {
		T temp = bucket[i]; 
		int j = i - 1;

		
		while (j >= 0 && cmp(bucket[j], temp) > 0) {
			bucket[j + 1] = bucket[j];
			j--;
		}
		bucket[j + 1] = temp; 
	}
}

void bucket_sort(int* array, int n, int m)
{
	vector<vector<int>> buckets(n);
	double bucket_size = m / (double) n;
	for (int i = 0; i < n; i++) {
		
		int index = static_cast<int>(array[i]/ bucket_size);


		if (index >= n) index = n - 1;

		buckets[index].push_back(array[i]);
	}
	for (int i = 0; i < n; i++) {
		insertionSort(buckets[i]);
	}
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (int value : buckets[i]) {
			array[index++] = value;
		}
	}
}


template<typename T>
void bucket_sort(T* array, int n, double m, double(*key)(T), double(*cmp)(T, T)) {
	vector<vector<T>> buckets(n); 
	double bucket_size = m / n;   

	
	for (int i = 0; i < n; i++) {
		double key_value = key(array[i]);
		int index = static_cast<int>(key_value / bucket_size);


		if (index >= n) index = n - 1;

		buckets[index].push_back(array[i]);
	}

	
	for (int i = 0; i < n; i++) {
		insertionSort(buckets[i], cmp);
	}

	
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (T value : buckets[i]) {
			array[index++] = value;
		}
	}
}
int random_int() {
	return ((rand() << 15) + rand()) % 10000;;
}
template<typename T>
string print_table(T* array, int n)
{
	std::string krokiet = "Elementy: |";

	if (n > 10)
	{
		for (int i = 0;i < 5;i++) krokiet += std::to_string(array[i]) + "|";
		krokiet += "...";
		for (int i = n - 5;i < n;i++) krokiet += std::to_string(array[i]) + "|";
	}
	else
	{
		for (int i = 0;i < n;i++) krokiet += std::to_string(array[i]) + "|";
	}
	return krokiet;
}
template<typename T>
string print_table(T* array,int n, string(*see)(some_object*))
{
	std::string krokiet = "Elementy:  |";

	if (n > 10)
	{
		for (int i = 0;i < 5;i++) krokiet += see(array[i]);
		krokiet += "...|";
		for (int i = n - 5;i < n;i++) krokiet += see(array[i]);
	}
	else
	{
		for (int i = 0;i < n;i++) krokiet += see(array[i]);
	}
	return krokiet;
}
template <typename T>
string check_tables(T* array1, T* array2, T* array3, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (array1[i] != array2[i] || array2[i] != array3[i] || array1[i] != array3[i])
			return "Tablice się nie zgadzają";
	}
	return "Tablice sa zgodne";
}
template <typename T>
string check_tables(T* array1, T* array2, int n, double(*cmp)(T, T)) 
{
	
	for (int i = 0; i < n; i++)
	{
		if (cmp(array1[i], array2[i])!=0)
			return "Tablice się nie zgadzają";
	}
	return "Tablice sa zgodne";
}

int main_ints()
{
	srand(0);
	const int MAX_ORDER = 7; 
	const int m = (int)pow(10, 7); 
	for (int o = 1; o <= MAX_ORDER; o++)
	{
		const int n = (int)pow(10, o); 
		int* array1 = new int[n];
		for (int i = 0; i < n; i++)
		{
			int rand_val = random_int();
				array1[i] = rand_val;
		}
		cout << print_table(array1,n);
		cout << " Nieposortowana tablica. Liczba elementow: "<<n << endl;
			int* array2 = new int[n];
		int* array3 = new int[n];
		memcpy(array2, array1, n * sizeof(int)); 
		memcpy(array3, array1, n * sizeof(int)); 
		
		
		clock_t t1 = clock();
		counting_sort(array1, n, m);
		clock_t t2 = clock();
		double time1 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
		cout << print_table(array1, n);
		cout << " Wykorzystany Counting Sort.\n Posortowano: " << n << " Elementow. Czas to: " << time1 << " sekundy" << endl;
		
		
			t1 = clock();
		tablica< int >* bh = new tablica < int >(array2, n, true); 
			bh -> sort();
		t2 = clock();
		double time2 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
		cout << print_table(array2, n);
		cout << " Wykorzystany Heap Sort.\n Posortowano: " << n << " Elementow. Czas to: " << time2 << " sekundy" << endl;
			t1 = clock();
		bucket_sort(array3, n, m); 
		t2 = clock();
		double time3 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
		cout << print_table(array3, n);
		cout << " Wykorzystany Bucket Sort.\n Posortowano: " << n << " Elementow. Czas to: " << time3 << " sekundy" << endl;
		cout << check_tables(array1, array2, array3,n)<<endl;
		cout << "=============================================================" << endl;
		delete[] array1;
		delete[] array2;
		delete[] array3;
	}
	return 0;
}
int main_some_objects()
{
	srand(0);
	const int MAX_ORDER = 7;
	const double m_double = (double)pow(2, 30);
	for (int o = 1; o <= MAX_ORDER; o++)
	{
		const int n = (int)pow(10, o);
		some_object** array1 = new some_object * [n];
		for (int i = 0; i < n; i++)
		{
			some_object* so = new some_object();
			double a = ((rand() << 15) + rand()) / m_double;
			so->field_1 = std::floor(a * 1000000) / 1000000.0;
			so->field_2 = 'a' + rand() % 26;
			array1[i] = so;
		}
		cout << print_table(array1, n, see);
		cout << " Nieposortowana tablica. Liczba elementow: " << n << endl;
		some_object** array2 = new some_object * [n];
		memcpy(array2, array1, n * sizeof(some_object*));
		clock_t t1 = clock();
		tablica < some_object* >* bh = new tablica < some_object* >(array1, n, some_objects_cmp, true);
		bh->sort(some_objects_cmp);
		clock_t t2 = clock();
		double time2 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
		cout << print_table(array1, n,see);
		cout << " Wykorzystany Heap Sort.\n Posortowano: " << n << " Elementow. Czas to: " << time2 << " sekundy" << endl;
		 t1 = clock();
		bucket_sort <some_object*>(array2, n, 1.0, some_objects_key_double, some_objects_cmp); 
		 t2 = clock();
		double time3 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
		cout << print_table(array2, n, see);
		cout << " Wykorzystany Bucket Sort.\n Posortowano: " << n << " Elementow. Czas to: " << time3 << " sekundy" << endl;
		cout << check_tables(array1, array2, n,some_objects_cmp) << endl;
		cout << "=============================================================" << endl;
		delete[] array1;
		delete[] array2;
	}
	return 0;
}

int main()
{
	//main_ints();

	main_some_objects();

}
///////////////////////////////////////

#ifndef HEADER_H
#define HEADER_H

using namespace std;
#include <iostream>
#include<string>
#include <ctime>
#include <cmath>

template<typename T>
class tablica
{

	int rozmiar;
	int maks = 2;
	T* array;
public:
	tablica() {
		rozmiar = 0;
		array = new T[maks];
	};
	~tablica() {
		clear();
	}

	tablica(T* existingArray, int n, bool fixed)
	{
		array = existingArray;
		rozmiar = n;
		maks = n;

		if (fixed) heapifyBottomDown();
		else heapifyTopDown();

	}
	tablica(T* existingArray, int n, double(*cmp)(T, T), bool fixed)
	{
		array = existingArray;
		rozmiar = n;
		maks = n;

		if (fixed) heapifyBottomDown(cmp);
		else heapifyTopDown(cmp);

	}

	void heapifyTopDown(double(*cmp)(T, T))
	{
		for (int i = 0;i < rozmiar;i++)
			heapifyUp(i, cmp);
	}
	void heapifyBottomDown(double(*cmp)(T, T))
	{
		for (int i = (rozmiar - 2) / 2;i >= 0;i--)
			heapifyDown(i, cmp);
	}
	void heapifyTopDown()
	{
		for (int i = 0;i < rozmiar;i++)
			heapifyUp(i);
	}
	void heapifyBottomDown()
	{
		for (int i = (rozmiar - 2) / 2;i >= 0;i--)
			heapifyDown(i);
	}
	void sort(double(*cmp)(T, T))
	{
		int nasz_rozmiar = rozmiar;
		while (rozmiar > 1) {
			swap(array[0], array[rozmiar - 1]);
			rozmiar--;
			heapifyDown(0, cmp);
		}
		rozmiar = nasz_rozmiar;
	}
	void sort()
	{
		int nasz_rozmiar = rozmiar;
		while (rozmiar > 1) {
			swap(array[0], array[rozmiar - 1]);
			rozmiar--;
			heapifyDown(0);
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

	void heapifyUp(int index, double(*cmp)(T, T)) {
		if (index == 0)
			return;

		int parent = (index - 1) / 2;

		if (cmp(array[index], array[parent]) > 0) {

			T temp = array[index];
			array[index] = array[parent];
			array[parent] = temp;
			heapifyUp(parent, cmp);
		}

	}
	T del_root1(double(*cmp)(T, T))
	{
		T root = array[0];
		array[0] = array[rozmiar - 1];
		rozmiar--;
		heapifyDown(0, cmp);
		return root;
	}
	void heapifyDown(int index, double(*cmp)(T, T)) {
		if (index >= rozmiar) return;
		int leftchild = 2 * index + 1;
		int rightchild = 2 * index + 2;
		int largest = index;
		if (leftchild < rozmiar && cmp(array[leftchild], array[largest]) > 0)
			largest = leftchild;
		if (rightchild < rozmiar && cmp(array[rightchild], array[largest]) > 0)
			largest = rightchild;

		if (largest != index) {
			swap(array[index], array[largest]);
			heapifyDown(largest, cmp);
		}
	}



	std::string print()
	{
		std::string krokiet = "Pojemnosc: " + std::to_string(maks) + " Liczba Elementow: " + std::to_string(rozmiar) + "\nElementy: ";

		if (rozmiar > 10)
		{
			for (int i = 0;i < 5;i++) krokiet += std::to_string(array[i]) + "|";
			krokiet += "...";
			for (int i = rozmiar - 5;i < rozmiar;i++) krokiet += std::to_string(array[i]) + "|";
		}
		else
		{
			for (int i = 0;i < rozmiar;i++) krokiet += std::to_string(array[i]) + "|";
		}
		return krokiet;
	}

	std::string print(std::string(*see)(T*))
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
		maks = 2;
	}



};





#endif

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
	binary_heap(T* existingArray, int n, T origin, bool(*cmp)(T, T,T), bool fixed)
	{
		array = existingArray;
		rozmiar = n;
		maks = n;

		if (fixed) heapifyBottomDown(origin,cmp);
		else heapifyTopDown(origin,cmp);

	}

	void heapifyTopDown(T origin,bool(*cmp)(T, T,T))
	{
		for (int i = 0;i < rozmiar;i++)
			heapifyUp(i,origin, cmp);
	}
	void heapifyBottomDown(T origin,bool(*cmp)(T, T, T))
	{
		for (int i = (rozmiar - 2) / 2;i >= 0;i--)
			heapifyDown(i,origin, cmp);
	}

	void sort(T origin,bool (*cmp)(T, T,T))
	{
		int nasz_rozmiar = rozmiar;
		while (rozmiar > 1) {
			swap(array[0], array[rozmiar - 1]);
			rozmiar--;
			heapifyDown(0,origin, cmp);
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

	void heapifyUp(int index,T origin, bool(*cmp)(T,T,T)) {
		if (index == 0)
			return;

		int parent = (index - 1) / 2;

		if (cmp(array[index], array[parent],origin)) {

			T temp = array[index];
			array[index] = array[parent];
			array[parent] = temp;
			heapifyUp(parent,origin, cmp);
		}

	}
	T del_root1(double(*cmp)(T,T,T))
	{
		T root = array[0];
		array[0] = array[rozmiar - 1];
		rozmiar--;
		heapifyDown(0, cmp);
		return root;
	}
	void heapifyDown(int index,T origin, bool(*cmp)(T, T,T)) {
		if (index >= rozmiar) return;
		int leftchild = 2 * index + 1;
		int rightchild = 2 * index + 2;
		int largest = index;
		if (leftchild < rozmiar && cmp(array[leftchild], array[largest],origin))
			largest = leftchild;
		if (rightchild < rozmiar && cmp(array[rightchild], array[largest],origin))
			largest = rightchild;

		if (largest != index) {
			swap(array[index], array[largest]);
			heapifyDown(largest,origin, cmp);
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

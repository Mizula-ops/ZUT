
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
		tablica(T * existingArray, int n, double(*cmp)(T, T), bool fixed)
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
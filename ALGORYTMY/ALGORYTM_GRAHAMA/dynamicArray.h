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


	void set_min(double(*cmp)(T,T))
	{
		T tmp=array[0];
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
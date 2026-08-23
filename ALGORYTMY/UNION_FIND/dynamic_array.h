#ifndef DYNAMIC_ARRAY_H
#define DYNAMI_ARRAY_H 
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
		maks = 100;
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
	int get_size()
	{
		return rozmiar;
	}
	double suma(double(*give)(T, int))
	{
		double wynik=0;
		for (int i=0;i < rozmiar;i++)
		{
			wynik = wynik + give_some_objects(array[i], 3);
		}
		return wynik;
	}


	std::string print_da(std::string(*see)(T))
	{

	
			std::string krokiet = "Elementy:  |";
			
			if (rozmiar > 21)
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
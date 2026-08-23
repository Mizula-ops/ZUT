//ALGO2 IS1 211B LAB02
//Artur Mizuła
//ma55831@zut.edu.pl

#include <iostream>
#include<string>
#include <ctime>
#include <cmath>
struct some_object
{
	int field_1;
	char field_2;
};
some_object* generate_some_object()
{
	some_object* so = new some_object();
	so->field_1 = rand() % 10000;
	so->field_2 = 'a' + (rand() % 26);
	return so;
}
 int some_objects_cmp(some_object* so1, some_object* so2)
{
	int diff = so1->field_1 - so2->field_1;
	if (diff != 0)
		return diff;
	return so1->field_2 - so2->field_2;
}
std::string see(some_object* so1)
{
	return std::to_string(so1->field_1)+""+ so1->field_2 + "|";
}
template<typename T>
class tablica
{
	
	int maks;
	int rozmiar;
	T* array;
public:
	tablica() {
		maks =2;
		rozmiar=0;
		array = new T[maks];
	};
	~tablica() {
		clear();
	}
	void backpush(T data)
	{
		if (rozmiar< maks)
		{
			array[rozmiar] = data;
			rozmiar++;
		}
		else
		{
			maks *= 2;
			T*newArray = new T[maks];
			for (int i = 0;i <= rozmiar;i++) {
				newArray[i] = array[i];
				
			}
			newArray[rozmiar] = data;
			rozmiar++;
			delete[] array;
			array = newArray;
		}

	}
	
	std::string print()
	{
		std::string krokiet = "Pojemnosc: " + std::to_string(maks) + " Liczba Elementow: " + std::to_string(rozmiar) + "\nElementy: ";

		if (rozmiar > 10)
		{
			for (int i = 0;i < 5;i++) krokiet += std::to_string(array[i]) + "|";
			krokiet += "...";
				for (int i = rozmiar-5;i<rozmiar;i++) krokiet += std::to_string(array[i]) + "|";
		}
		else
		{
			for (int i = 0;i < rozmiar;i++) krokiet += std::to_string(array[i]) + "|";
		}
		return krokiet;
	}
	
	std::string print(std::string(*see)(some_object*))
	{
		std::string krokiet = "Pojemnosc: " + std::to_string(maks) + " Liczba Elementow: " + std::to_string(rozmiar) + "\nElementy:  |";

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
	T index(int which)
	{
		if (which > maks) std::cout << "Indeks poza skala \n";
		else if (which>rozmiar-1) std::cout << "Szufladka pusta \n";
		else return array[which];
	}
	void change(int which, T data)
	{
		if (which > maks) std::cout << "Indeks poza skala \n";
		else if (which > rozmiar - 1) std::cout << "Szufladka pusta \n";
		else  array[which]=data;
	}
	void bubble_sort()
	{
		
		for(int i=0;i<=rozmiar-1;i++)
			for (int j = 1;j <= rozmiar - i-1;j++)
			{
				if (array[j-1] > array[j])
				{
					T tmp = array[j-1];
					array[j-1] = array[j];
					array[j] = tmp;
				}
			}
	}
	void bubble_sort(int(*cmp)(some_object*,some_object*))
	{

		for (int i = 0;i <= rozmiar - 1;i++)
			for (int j = 1;j <= rozmiar - i - 1;j++)
			{
				if (cmp(array[j - 1], array[j])>0)
				{
					some_object * tmp = array[j - 1];
					array[j - 1] = array[j];
					array[j] = tmp;
				}
			}
	}
};


int main() {
	tablica<some_object*>* da = new tablica<some_object*>();

	const int order = 7;
	const int n = static_cast<int>(pow(10, order)); 
	clock_t t1 = clock();
	double max_time_per_element = 0.0; 
	int max_time_index = -1; 
	

	for (int i = 0; i < n; i++) {
		some_object* so = generate_some_object();

		
		clock_t t1_element = clock();
		da->backpush(so);
		clock_t t2_element = clock();

		
		double time_per_element = static_cast<double>(t2_element - t1_element) / CLOCKS_PER_SEC;

		
		if (time_per_element > max_time_per_element) {
			max_time_per_element = time_per_element;
			max_time_index = i;
			std::cout << "Nowy najgorszy czas dodania elementu: " << max_time_per_element
				<< " sek., dla indeksu: " << max_time_index << std::endl;
		}
	}
	
	clock_t t2 = clock(); 

	double total_time = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
	double amortized_time_per_element = total_time / n;

	
	std::cout<<da->print(see);
	std::cout << std::endl;

	std::cout << "Czas calkowity: " << total_time << " sek." << std::endl;
	std::cout << "Zamortyzowany czas na element: " << amortized_time_per_element << " sek." << std::endl;
	std::cout << "Maksymalny czas na pojedyncze dodanie: " << max_time_per_element << " sek. dla indeksu " << max_time_index << std::endl;

	
	da->clear(); 
	delete da;
	
	return 0;
}



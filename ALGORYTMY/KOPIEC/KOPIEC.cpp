//ALGO2 IS1 211B LAB05
//Artur Mizuła
//ma55831@zut.edu.pl
using namespace std;
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
	so->field_1 = ((rand() << 15) + rand()) % 5;
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
	return std::to_string(so1->field_1) + "" + so1->field_2 + "|";
}
template<typename T>
class tablica
{

	int rozmiar; 
	int maks=2; 
	T* array;
public:
	tablica() {
		rozmiar = 0;
		array = new T[maks];
	};
	~tablica() {
		clear();
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
	void add(T data, int(*cmp)(T, T))
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
	
	void heapifyUp(int index, int(*cmp)(T, T)) {
		if (index == 0)
			return;

		int parent = (index - 1) / 2;

		if (cmp(array[index], array[parent])>0) {

			T temp = array[index];
			array[index] = array[parent];
			array[parent] = temp;
			heapifyUp(parent,cmp);
		}

	}
	T del_root1(int(*cmp)(T, T))
	{
		T root = array[0];
		array[0] = array[rozmiar - 1];
		rozmiar--;
		heapifyDown(0,cmp);
		return root;
	}
	void heapifyDown(int index, int(*cmp)(T, T)) {
		if (index >= rozmiar) return;  
		int leftchild = 2 * index + 1;
		int rightchild = 2 * index + 2;

	
		//if (leftchild >= rozmiar) return;

	
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
	
	
	
};

int main1() {
	std::srand(static_cast<unsigned>(std::time(nullptr)));


	tablica<double> bracia;
	
	for (int i = 0; i < 10; i++) {
		int randomValue = std::rand()%100;
		std::cout << "Random Value " << i  << ": " << randomValue << std::endl;
		bracia.add(randomValue);
	}
	
	cout<<bracia.print();
	bracia.del_root();
	cout << bracia.print();
	return 0;
}

int main()
{

	const int MAX_ORDER = 6;
	tablica < some_object* >* bh = new tablica < some_object* >();
	for (int o = 1; o <= MAX_ORDER; o++)
	{
		const int n = pow(10, o);

		clock_t t1_add = clock();
		for (int i = 0; i < n; i++) {
			some_object* so = generate_some_object();
			bh->add(so, some_objects_cmp);
		}
		cout << bh->print(see) << endl;
		clock_t t2_add = clock();
		double time_add = static_cast<double>(t2_add - t1_add) / CLOCKS_PER_SEC;
		std::cout << "Czas dodania: " << n << " elemntow to: " << time_add << " sek." << std::endl;
		
		clock_t t1 = clock();
		for (int i = 0; i < n; i++) {
			some_object* polled = bh->del_root1(some_objects_cmp);
			if(i<10)std::cout << "Usuniety element: " << see(polled) << std::endl;
			delete polled;
		}
		clock_t t2 = clock();
		double time = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
		std::cout << "Usunieto " << n << " elemntow w czasie " << time << " seconds" << std::endl;
		cout << bh->print(see) << endl;
		bh->clear();
		
		
	}
	delete bh;
	return 0;
}
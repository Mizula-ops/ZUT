//ALGO2 IS1 211B LAB08
//ARTUR MIZULA
//ma55831@zut.edu.pl
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include "heap.h"
#include "dynamic_array.h"
using namespace std;
struct some_object
{
    int field_1;
    int field_2;
    double field_3;
};
double some_objects_cmp1(some_object* so1, some_object* so2)
{
    return so1->field_3 - so2->field_3;
}
double give_some_objects(some_object* so1, int which)
{
    if (which == 1) {
        return so1->field_1;
    }
    else if (which == 2) {
        return so1->field_2;
    }
    else if (which == 3) {
        return so1->field_3;
    }
}
std::string see(some_object* so1)
{
    return std::to_string(so1->field_1) + " " + to_string(so1->field_2) + " " + to_string(so1->field_3) + "|";
}
template <typename T>
class UnionFind {
    int *parent;
   double *rank;
   int ile_find=0;
   
public:
    UnionFind(int n) {
        parent = new int[n];
        rank = new double[n];
        for (int i = 0;i < n;i++)
        {
            parent[i] = i;
            rank[i] = 0;
        }
            
        
    }
    int find_path_compression(int x) {
        
        if (parent[x] != x)
            parent[x] = find(parent[x]); 
        return parent[x];;
    }
    int find(int x) {
        
        while(parent[x] != x) {
            x = parent[x];
        }
        return x;
    }


    bool unite_by_rank(int x, int y) {
        int rootX = find_path_compression(x);
        ile_find++;
        int rootY = find_path_compression(y);
        ile_find++;
            if (rootX != rootY) {
                if (rank[rootX] > rank[rootY]) {
                    parent[rootY] = rootX;
                }
                else if (rank[rootX] < rank[rootY]) {
                    parent[rootX] = rootY;
                }
                else{
                    parent[rootY] = rootX;
                        rank[rootX]++;
                }
                return true;
            }
            return false;
    }
    bool unite(int x, int y) {
        int rootX = find(x);
        ile_find++;
        int rootY = find(y);
        ile_find++;
        if (rootX != rootY) {
            parent[rootY] = rootX; 
            return true;
        }
        return false;
    }

    int get_ile_find()
    {
        return ile_find;
    }
    

    string print_uf(int rozmiar)
    {
        std::string krokiet = "Elementy: ";

        if (rozmiar > 100)
        {
            for (int i = 0;i < 5;i++) krokiet += std::to_string(parent[i]) + "|";
            krokiet += "...";
            for (int i = rozmiar - 5;i < rozmiar;i++) krokiet += std::to_string(parent[i]) + " ranga:"+ std::to_string(rank[i])+ "|";
        }
        else
        {
            for (int i = 0;i < rozmiar;i++) krokiet += std::to_string(parent[i]) + " ranga:" + std::to_string(rank[i]) + "|";
        }
        return krokiet;
    }

};

int main_normal() {
    for (int i = 1;i < 4;i++)
    {
        fstream uchwyt;
        int n, liczba_krawedzi = 0;
        string linia;
        string file_name = "g" + to_string(i) + ".txt";
        uchwyt.open(file_name);
        if (!uchwyt.is_open()) return 0;
        if (getline(uchwyt, linia)) {
            n = stoi(linia);
        }

        int pomin = 0;
        while (pomin < n && getline(uchwyt, linia)) pomin++;

        if (getline(uchwyt, linia)) {
            liczba_krawedzi = stoi(linia);
        }

        some_object** array1 = new some_object * [liczba_krawedzi];

        for (int i = 0; i < liczba_krawedzi; i++) {
            some_object* so = new some_object();


            if (getline(uchwyt, linia)) {
                stringstream ss(linia);
                ss >> so->field_1 >> so->field_2 >> so->field_3;
                array1[i] = so;
            }

        }
        uchwyt.close();
        clock_t t1 = clock();
        tablica < some_object* >* bh = new tablica < some_object* >(array1, liczba_krawedzi, some_objects_cmp1, true);
        bh->sort(some_objects_cmp1);
        clock_t t2 = clock();
        double time_sort = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
        cout << bh->print_h(see) << endl;
        cout << "Posortowano " << liczba_krawedzi << " krawedzi. Uzyto Heap Sort. Czas to: " << time_sort << " sekund" << endl << endl;

        UnionFind<some_object>* uf = new UnionFind<some_object>(n);
        dynamic_array<some_object*>* mst = new dynamic_array<some_object*>();

        t1 = clock();
        for (int i = 0;i < liczba_krawedzi;i++)
        {

            double object1 = bh->give_some_array(i, 1, give_some_objects);
            double object2 = bh->give_some_array(i, 2, give_some_objects);
            double weights = bh->give_some_array(i, 3, give_some_objects);
            if (uf->unite(object1, object2))
            {
                mst->backpush(bh->give_array(i));
            }
        }
        t2 = clock();
        double time_mst = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
        cout << mst->print_da(see) << endl;
        cout << "Stworzono Drzewo MST. Czas to: " << time_mst << " sekund. Wywolano: "<<uf->get_ile_find()<<" razy funkcje find."<< endl;
        cout <<"Ilosc krawedzi: "<<mst->get_size()<<" Suma wag: " << mst->suma(give_some_objects)<<endl;
        
        cout << "=======================================================================================" << endl;
        delete[] array1;
        
       
    }
    return 0;
}
int main_optimal()
{
    for (int i = 1;i < 4;i++)
    {
        fstream uchwyt;
        int n, liczba_krawedzi = 0;
        string linia;
        string file_name = "g" + to_string(i) + ".txt";
        uchwyt.open(file_name);
        if (!uchwyt.is_open()) return 0;
        if (getline(uchwyt, linia)) {
            n = stoi(linia);
        }

        int pomin = 0;
        while (pomin < n && getline(uchwyt, linia)) pomin++;

        if (getline(uchwyt, linia)) {
            liczba_krawedzi = stoi(linia);
        }

        some_object** array1 = new some_object * [liczba_krawedzi];

        for (int i = 0; i < liczba_krawedzi; i++) {
            some_object* so = new some_object();


            if (getline(uchwyt, linia)) {
                stringstream ss(linia);
                ss >> so->field_1 >> so->field_2 >> so->field_3;
                array1[i] = so;
            }

        }
        uchwyt.close();
        clock_t t1 = clock();
        tablica < some_object* >* bh = new tablica < some_object* >(array1, liczba_krawedzi, some_objects_cmp1, true);
        bh->sort(some_objects_cmp1);
        clock_t t2 = clock();
        double time_sort = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
        cout << bh->print_h(see) << endl;
        cout << "Posortowano " << liczba_krawedzi << " krawedzi. Uzyto Heap Sort. Czas to: " << time_sort << " sekund" << endl << endl;

        UnionFind<some_object>* uf = new UnionFind<some_object>(n);
        dynamic_array<some_object*>* mst = new dynamic_array<some_object*>();

        t1 = clock();
        for (int i = 0;i < liczba_krawedzi;i++)
        {

            double object1 = bh->give_some_array(i, 1, give_some_objects);
            double object2 = bh->give_some_array(i, 2, give_some_objects);
            double weights = bh->give_some_array(i, 3, give_some_objects);
            if (uf->unite_by_rank(object1, object2))
            {
                mst->backpush(bh->give_array(i));
            }
        }
        t2 = clock();
        double time_mst = static_cast<double>(t2 - t1) / CLOCKS_PER_SEC;
        cout << mst->print_da(see) << endl;
        cout << "Stworzono Drzewo MST. Czas to: " << time_mst << " sekund. Wywolano: " << uf->get_ile_find() << " razy funkcje find." << endl;
        cout << "Ilosc krawedzi: " << mst->get_size() << " Suma wag: " << mst->suma(give_some_objects) << endl;

        cout << "=======================================================================================" << endl;
        delete[] array1;


    }
    return 0;
}
int main() {
    main_optimal();
   //main_normal();
    return 0;
} 

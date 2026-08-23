//ALGO2 IS1 211B LAB06
//Artur Mizuła
//ma55831@zut.edu.pl
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

template <typename V>
struct Node {
    string key;
    V value;
    Node* next;

    Node(string k,V v) : key(k), value(v), next(nullptr) {}
};


template <typename V>
class HashTable {
private:
    Node<V>** table; 
    int capacity; 
    int size;     
    double loadFactor;

    
    int hash(string key) const {
        int hashValue = 0;
        int power = 1;
        for (char ch : key) {
            hashValue = (hashValue + (ch * power) % capacity) % capacity;
            power = (power * 31) % capacity;
        }
        return hashValue;
    }

    
    void rehash() {
        int oldCapacity = capacity;
        capacity *= 2;
        Node<V>** newTable = new Node<V>*[capacity]();

        for (int i = 0; i < oldCapacity; i++) {
            Node<V>* current = table[i];
            while (current) {
                int newIndex = hash(current->key);
                Node<V>* next = current->next;
                current->next = newTable[newIndex];
                newTable[newIndex] = current;
                current = next;
            }
        }
        delete[] table;
        table = newTable;
    }

public:
    
    HashTable(int initialCapacity = 8, double loadFactor = 0.75)
        : capacity(initialCapacity), size(0), loadFactor(loadFactor) {
        table = new Node<V>*[capacity]();
    }

   
    ~HashTable() {
        clear();
        delete[] table;
    }

    void insert(string key, V value) {
        int index = hash(key);
        Node<V>* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value; 
                return;
            }
            current = current->next;
        }

        Node<V>* newNode = new Node<V>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        ++size;

        if (static_cast<double>(size) / capacity > loadFactor) {
            rehash();
        }
    }

  
    Node<V>* find(string key) {
        int index = hash(key);
        Node<V>* current = table[index];
        while (current) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

   
    bool erase(string key) {
        int index = hash(key);
        Node<V>* current = table[index];
        Node<V>* prev = nullptr;
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    table[index] = current->next;
                }
                delete current;
                --size;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    
    void clear() {
        for (int i = 0; i < capacity; i++) {
            Node<V>* current = table[i];
            while (current) {
                Node<V>* next = current->next;
                delete current;
                current = next;
            }
            table[i] = nullptr;
        }
        size = 0;
    }

    string toString() const {
        string result = "hash table:\n";
        int ile = 0;
        for (int i = 0; i < capacity; i++) {
           
            if (table[i]) {
                ile++;
                    if (ile > 9) break;
                result += to_string(i) + ": ";
                Node<V>* current = table[i];
                while (current) {
                    result += current->key + " -> " + to_string(current->value) + "; ";
                    current = current->next;
                }
                result += "\n";
            }
        }
        ile = 0;
        for (int i = capacity-1; i >0; i--) {
            if (table[i]) {
                ile++;
                if (ile > 9) break;
                result += to_string(i) + ": ";
                Node<V>* current = table[i];
                while (current) {
                    result += current->key + " -> " + to_string(current->value) + "; ";
                    current = current->next;
                }
                result += "\n";
            }
        }
        return result;
    }
    string toString1() const {
        string result = "hash table:\n";
        
        for (int i = 0; i < capacity; i++) {

            if (table[i]) {
               
              
                result += to_string(i) + ": ";
                Node<V>* current = table[i];
                while (current) {
                    result += current->key + " -> " + to_string(current->value) + "; ";
                    current = current->next;
                }
                result += "\n";
            }
        }
      
        return result;
    }
    
    void printStats() const {
        int nonEmptyLists = 0;
        int minLength = 1;
        int maxLength = 0;
        int totalLength = 0;

        for (int i = 0; i < capacity; ++i) {
            int length = 0;
            Node<V>* current = table[i];
            while (current) {
                ++length;
                current = current->next;
            }
            if (length > 0) {
                nonEmptyLists++;
                totalLength += length;
                minLength = std::min(minLength, length);
                maxLength = std::max(maxLength, length);
            }
        }

        std::cout << "Statystyki:\n";
        std::cout << "Listy z zawartoscia: " << nonEmptyLists << "\n";
        std::cout << "Najmniejszy rozmair listy: " << minLength<< "\n";
        std::cout << "Najwiekszy rozmiar listy: " << maxLength << "\n";
        std::cout << "Maksymalna pojemnosc: " << totalLength << "\n";
    }
};


    string random_key(size_t length = 6) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const size_t maxIndex = sizeof(charset) - 1;
    std::string key;
    for (size_t i = 0; i < length; ++i) {
        key += charset[rand() % maxIndex];
    }
    return key;
}
int main1() {
    HashTable<int> ht;
    
    for (int i = 0;i < 10;i++)
    {
        ht.insert(random_key(1), i);
        
    }
    string key1;
    key1 += 'a';
    ht.insert(key1, 11);
    cout << ht.toString1() << endl;
    Node<int> *entry=ht.find(key1);
    
    cout<< to_string(ht.find(key1)->value);
    ht.erase(key1);
    cout << ht.toString1() << endl;
}
int main() {
    srand(static_cast<unsigned>(time(0)));

    const int MAX_ORDER = 7; 
    HashTable<int>* ht = new HashTable<int>();
    
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);

        clock_t t1 = clock();
        for (int i = 0; i < n; i++)
            ht->insert(random_key(6), i); 
        clock_t t2 = clock();

        
        cout << ht->toString() << std::endl;
        double time = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
        cout << "Dodano: " << n << " Elementow. Czas to: " << time << " sekundy" <<endl;

        
        const int m = pow(10, 4);
        int hits = 0;
        t1 = clock();
        for (int i = 0; i < m; i++)
        {
            Node<int>* entry = ht->find(random_key(6)); 
            if (entry != nullptr)
                hits++;
           
        }
        t2 = clock();
       
        double time1 = (t2 - t1) / static_cast<double>(CLOCKS_PER_SEC);
        cout << "Trafiono: " << hits << " Elementow. Czas to: " << time1 << " sekundy" << endl;
       
        ht->printStats();
        ht -> clear(); 
    }
    delete ht;
    return 0;

    }
    
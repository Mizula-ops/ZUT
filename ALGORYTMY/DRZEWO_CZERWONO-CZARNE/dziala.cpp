//ALGO2 IS1 211B LAB04
//Artur Mizuła
//ma55831@zut.edu.pl
#include <iostream>
#include <string>
#include <vector>
#include <cmath> 
#include <ctime>
using namespace std;
struct some_object
{
    int field_1;
    char field_2;
};
some_object* generate_some_object()
{
    some_object* so = new some_object();

    so->field_1 = ((rand() <<15) + rand()) % 100000;
    so->field_2 = 'a' + (rand() % 26);
    return so;
}
some_object* generate_my_object()
{
    some_object* so = new some_object();
    so->field_1 = 1;
    so->field_2 = 'z';
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
    return std::to_string(so1->field_1) + "" + so1->field_2;
}
template <typename T>
struct Node {
    Node* left;
    Node* right;
    Node* parent;
    int indeks;
    bool isRed;
    T data;
    Node(T data, int index) : left(nullptr), right(nullptr), parent(nullptr), data(data), indeks(index), isRed(true) {}
};
template <typename T>
class Tree {

    Node<T>* root;
    int size;


public:

    Tree() : root(nullptr), size(0) {}
    unsigned int get_size()
    {
        return size;
    }

    void add(T data) {
        Node<T>* current = new Node<T>(data, size);
        current->indeks = ++size;

        if (!root)
        {
            root = current;
            root->isRed = false;
        }
        else {
            Node<T>* p = root;
            while (p != nullptr) {
                if (current->data > p->data) {
                    if (p->right == nullptr) {
                        p->right = current;
                        current->parent = p;
                        break;
                    }
                    else {
                        p = p->right;
                    }
                }
                else if (current->data < p->data) {
                    if (p->left == nullptr) {
                        p->left = current;
                        current->parent = p;
                        break;
                    }
                    else {
                        p = p->left;
                    }
                }
                else {

                    delete current;
                    return;
                }
            }
            check(current);
        }
    }
    void add(T data, int(*cmp)(T, T))
    {

        Node<T>* newNode = new Node<T>(data, size++);


        if (!root)
        {
            root = newNode;
            root->isRed = false;
        }
        else {
            Node<T>* current = root;


            while (true) {

                if (cmp(data, current->data) < 0)
                {
                    if (current->left == nullptr)
                    {
                        current->left = newNode;
                        newNode->parent = current;
                        break;
                    }
                    current = current->left;
                }

                else if (cmp(data, current->data) > 0)
                {
                    if (current->right == nullptr)
                    {
                        current->right = newNode;
                        newNode->parent = current;
                        break;
                    }
                    current = current->right;
                }else {

                    delete newNode;
                    size--;
                    return;
                }


            }

        }
        check(newNode);
    }
    void check(Node<T>* node)
    {
        while (node != root && node->parent->isRed == true) {

            Node<T>* uncle;

            if (node->parent == node->parent->parent->left) {

                uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->isRed == true) {

                    node->parent->isRed = false;
                    uncle->isRed = false;
                    node->parent->parent->isRed = true;
                    node = node->parent->parent;

                }
                else {

                    if (node == node->parent->right) {

                        node = node->parent;
                        rotation_left(node);

                    }

                    node->parent->isRed = false;
                    node->parent->parent->isRed = true;
                    rotation_right(node->parent->parent);

                }

            }
            else {

                uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->isRed == true) {
                    node->parent->isRed = false;
                    uncle->isRed = false;
                    node->parent->parent->isRed = true;
                    node = node->parent->parent;

                }
                else {

                    if (node == node->parent->left) {

                        node = node->parent;

                        rotation_right(node);

                    }

                    node->parent->isRed= false;

                    node->parent->parent->isRed = true;

                    rotation_left(node->parent->parent);
                    
                }

            }

        }

        root->isRed = false;

    }
   
       
    void rotation_right(Node<T>* node)
    {
        
  
        Node<T>* newParent = node->left;

        node->left = newParent->right;

        if (newParent->right != nullptr) {

            newParent->right->parent = node;

        }

        newParent->parent = node->parent;

        if (node->parent == nullptr) {

            root = newParent;

        }
        else if (node == node->parent->right) {

            node->parent->right = newParent;

        }
        else {

            node->parent->left = newParent;

        }

        newParent->right = node;

        node->parent = newParent;
    }
    void rotation_left(Node<T>* node)
    {


        Node<T>* newParent = node->right;

        node->right = newParent->left;

        if (newParent->left != nullptr) {

            newParent->left->parent = node;
        }

        newParent->parent = node->parent;

        if (node->parent == nullptr) {

            root = newParent;

        }
        else if (node == node->parent->left) {

            node->parent->left = newParent;

        }
        else {

            node->parent->right = newParent;

        }

        newParent->left = node;

        node->parent = newParent;
    }
    

    void remove(Node<T>* root) {
        if (root == nullptr)
        {
            return;
        }
        remove(root->left);
        remove(root->right);

        delete root;
        size--;

    }
    Node <T>* find(T value)
    {

        Node<T>* p = root;
        while (p)
        {
            if (p->data == value) return p;
            else if (value < p->data)
            {
                p = p->left;
            }
            else if (value > p->data)
            {
                p = p->right;
            }
        }
        return nullptr;
    }
    Node<T>* find(T value, int (*cmp)(T, T))
    {
        Node<T>* tmp = new Node<T>(value,0);
        Node<T>* p = root;
        while (p)
        {
            int result = cmp(p->data, tmp->data);
            if (result == 0) return p;
            else if (result < 0)
            {
                p = p->right;
            }
            else
            {
                p = p->left;
            }
        }
        return NULL;
    }
    string preorder()
    {

        string krokiet;
        preorder_helper(root, krokiet);
        return krokiet;

    }
    void preorder_helper(Node <T>* p, string& krokiet)
    {
        if (p != nullptr)
        {
            krokiet += " " + to_string(p->data);
            preorder_helper(p->left, krokiet);
            preorder_helper(p->right, krokiet);
        }
    }

    string preorder(string(*see)(T))
    {

        string krokiet;
        preorder_helper(root, krokiet, see);
        return krokiet;

    }
    void preorder_helper(Node <T>* p, string& krokiet, string(*see)(T))
    {
        if (p != nullptr)
        {
            krokiet += " " + see(p->data);
            preorder_helper(p->left, krokiet, see);
            preorder_helper(p->right, krokiet, see);
        }
    }
    string inorder()
    {

        string krokiet;

        inorder_helper(root, krokiet);

        return krokiet;
    }
    void inorder_helper(Node <T>* p, string& krokiet)
    {
        if (p != nullptr)
        {

            inorder_helper(p->left, krokiet);
            krokiet += " " + to_string(p->data);
            inorder_helper(p->right, krokiet);
        }
    }
    string inorder(string(*see)(T))
    {

        string krokiet;
        inorder_helper(root, krokiet, see);

        return krokiet;
    }
    void inorder_helper(Node <T>* p, string& krokiet, string(*see)(T))
    {
        if (p != nullptr)
        {

            inorder_helper(p->left, krokiet, see);
            krokiet += " " + see(p->data);
            inorder_helper(p->right, krokiet, see);
        }
    }

    void del(T value)
    {
        Node<T>* p = find(value);

        if (p != NULL)
        {

            if (!p->right && !p->left)
            {
                if (p == root) root = nullptr;
                else if (p->parent->left == p)
                    p->parent->left = nullptr;
                else
                    p->parent->right = nullptr;
                delete p;
                return;
            }
            else if (p->right && !p->left)
            {
                if (p == root)
                {
                    root = p->right;
                    root->parent = nullptr;
                }

                else if (p->parent->right == p)
                {
                    p->parent->right = p->right;
                }
                else
                {
                    p->parent->left = p->right;
                }
                p->right->parent = p->parent;
                delete p;
                return;
            }
            else if (!p->right && p->left)
            {
                if (p == root)
                {
                    root = p->left;
                    root->parent = nullptr;

                }

                else if (p->parent->right == p)
                {
                    p->parent->right = p->left;
                }
                else
                {
                    p->parent->left = p->left;
                }
                p->left->parent = p->parent;
                delete p;
                return;
            }
            else
            {

                Node<T>* help = p->left;
                while (help->right)
                {
                    help = help->right;
                }
                p->data = help->data;
                if (help->left)
                {
                    help->left->parent = help->parent;
                    if (help->parent->left = help)
                    {
                        help->parent->left = help->left;
                    }
                    else
                    {
                        help->parent->right = help->left;
                    }
                }
                else
                {

                    if (help->parent->left == help)
                    {
                        help->parent->left = nullptr;
                    }
                    else
                    {

                        help->parent->right = nullptr;
                    }
                }

                delete help;

            }
        }
        return;
    }
    int height()
    {
        int height = height_helper(root);
        return  height;
    }
    int height_helper(Node<T>* p)
    {
        if (p == nullptr)
        {
            return -1;
        }
        int left = height_helper(p->left);
        int right = height_helper(p->right);
        return  1 + max(left, right);
    }
    void deleteTree()
    {
        remove(root);
        root = nullptr;
        size = 0;
    }
   
    void print_helper(Node<T>* p, string& krokiet)
    {
        if (p != nullptr)
        {

            string parent = (p->parent != nullptr) ? to_string(p->parent->indeks) : "NULL";
            string left = (p->left != nullptr) ? to_string(p->left->indeks) : "NULL";
            string right = (p->right != nullptr) ? to_string(p->right->indeks) : "NULL";
            string color1;
            if (p->isRed == true) color1 = "RED";
            else  color1 = "BLACK";
            krokiet += "(" + to_string(p->indeks) + ": [" + color1 + ", p: " + parent + ", l : " + left + ", r : " + right + "], data: " + to_string(p->data) + ")\n";


            print_helper(p->left, krokiet);
            print_helper(p->right, krokiet);
        }
    }
    
        string print1() {
            string krokiet;
            print_helper(root, krokiet);
            return krokiet;
        }
        void print_helper(Node<T>* p, string& krokiet, std::string(*see)(T), long int& ile)
        {
            
            if (p != nullptr && ile<10)
            {

                string parent = (p->parent != nullptr) ? to_string(p->parent->indeks) : "NULL";
                string left = (p->left != nullptr) ? to_string(p->left->indeks) : "NULL";
                string right = (p->right != nullptr) ? to_string(p->right->indeks) : "NULL";
                ile++;
                string color1;
                if (p->isRed == true) color1 = "RED";
                else  color1 = "BLACK";
                krokiet += "(" + to_string(p->indeks) + ": [" + color1 + ", p: " + parent + ", l : " + left + ", r : " + right + "], data: " + see(p->data) + ")\n";
              
                print_helper(p->left, krokiet, see, ile);
                print_helper(p->right, krokiet, see, ile);
            }
        }

        string print1(std::string(*see)(some_object*)) {
            string krokiet;
            long int ile = 0;
            print_helper(root, krokiet, see, ile);
            return krokiet;
        }


};
 
    int main1()
    {
        

        Tree<double> brachu;
        brachu.add(9);
        brachu.add(15);
        brachu.add(2);
        brachu.add(39);
        brachu.add(14);
        brachu.add(35);
        brachu.add(16);
        brachu.add(12);
        brachu.add(38);
        brachu.add(54);
        
       // Node<double> braciak(23);
        //brachu.add(braciak.data);
       
        cout<<brachu.print1()<<endl;
        cout << brachu.preorder() << endl;
        cout << brachu.inorder() << endl;
       // int indeks = brachu.helper();
        //brachu.rotation_right(braciak3);
        //brachu.rotation_left(braciak3);
        return 0;
    } 
    int main()
    {
        const int MAX_ORDER = 7;
        Tree < some_object* >* rbt = new Tree < some_object*>();

        for (int o = 1; o <= MAX_ORDER; o++)
        {
            const int n = pow(10, o);

            clock_t t1 = clock();
            for (int i = 0; i < n; i++)

            {
                some_object* so = generate_some_object();
                rbt->add(so, some_objects_cmp);
               
            }
           
            clock_t t2 = clock();
            double add_time = double(t2 - t1) / CLOCKS_PER_SEC;


            const int m = pow(10, 4);
            int hits = 0;
            t1 = clock();
            for (int i = 0; i < m; i++)
            {
                some_object* so = generate_some_object();
                Node< some_object* >* result = rbt->find(so, some_objects_cmp);
                if (result != NULL)
                    hits++;
                delete so;
            }
            t2 = clock();

            double find_time = double(t2 - t1) / CLOCKS_PER_SEC;

            cout << rbt->print1(see);
            float h = rbt->height();
            float suma = h / log2(rbt->get_size() + 1);
            cout << "Wysokosc: " << h << endl;
            cout << "Rozmiar: " << rbt->get_size() << endl;
            cout << "h/log2(n+1): " << suma << endl;
            cout << "Dla " << n << " elementow:" << endl;
            cout << "Czas dodawania: " << add_time << " sekund" << endl;
            cout << "Czas wyszukiwania: " << find_time << " sekund" << endl;
            cout << "Liczba trafien: " << hits << endl;
            cout << "-------------------------------" << endl;
            rbt->deleteTree();
        }
        delete rbt;
        return 0;

    }
#include <fstream>
#include <istream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include<windows.h>

using namespace std;

bool isTestingTime = true;
int testSize = 1000;

struct Product
{
    int code;
    char name[25];

    Product()
    {
        this->code = 0;
        for (int i = 0; i < 25; i++)
            this->name[i] = '\0';
    }

    Product(string s)
    {
        stringstream stream(s);
        string part;

        stream >> part;
        this->code = stoi(part);

        stream >> part;
        for (int i = 0; i < part.size(); i++)
            this->name[i] = part[i];
        for (int i = part.size(); i < 25; i++)
            this->name[i] = '\0';
    }

    void Print() 
    {
        cout << "Продукт " << name << " с кодом " << code << "\n";
    }
};


class FileWorker
{
public:

    vector<Product> Create()
    {
        string s;
        vector<Product> products;

        ifstream file("txtfile.txt");
        while (!file.eof())
        {
            getline(file, s);
            //file.getline( >> s;
            products.push_back(Product(s));
        }
        file.close();
        Save(products);
        return products;
    }

    void Save(vector<Product>& products)
    {
        ofstream file;
        Product a;

        file.open("binfile.bin", ios::out);
        file.write((char*)&a, sizeof(Product));
        file.close();
        file.open("binfile.bin", ios::binary | ios::in | ios::app);

        for (int i = 0; i < products.size(); i++)
            file.write((char*)&products[i], sizeof(Product));
        file.close();
    }

    vector<Product> Load()
    {
        Product a;
        ifstream file("binfile.bin", ios::binary | ios::in);
        file.read((char*)&a, sizeof(Product));

        vector<Product> products;
        while (!file.eof())
        {
            file.read((char*)&a, sizeof(Product));
            products.push_back(a);
        }
        file.close();

        products.pop_back();
        return products;
    }

    Product Read(int index)
    {
        // Индексация с 1
        Product a;

        ifstream file("binfile.bin", ios::binary | ios::in);
        file.read((char*)&a, sizeof(Product));

        int i = 1;
        while (!file.eof())
        {
            file.read((char*)&a, sizeof(Product));
            if (i++ == index)
                return a;
        }
        return Product("-1 null");
    }

    void Add(vector<Product>& products, Product product)
    {
        products.push_back(product);
        Save(products);
    }
};



template <class T>
struct BNode
{
    T value;
    int key;
    BNode* left, * right;

    BNode(T val, int k, BNode* l, BNode* r)
    {
        value = val;
        key = k;
        left = l;
        right = r;
    }
};


template <typename T>
class BTree
{
    BNode<T>* root;

    int GetHeight(BNode<T>* node, int h)
    {
        if (node == NULL)
            return h;

        int leftH = GetHeight(node->left, h + 1);
        int rightH = GetHeight(node->right, h + 1);
        return rightH > leftH ? rightH : leftH;
    }

    void PrintLevel(vector<BNode<T>*> vec, int lev, int h, int prevProb)
    {
        if (lev == h + 1)
            return;

        string empty = "  ";
        vector<BNode<T>*> newVec;
        int prob = prevProb / 2, maxH = pow(2, h) - 1, j = 0, i = 0;

        for (; i < prob; i++)
            std::cout << empty;

        i = prob;
        do
        {
            if (j < vec.size())
            {
                if (vec[j++] != NULL)
                {
                    cout << vec[j - 1]->key;
                    newVec.push_back(vec[j - 1]->left);
                    newVec.push_back(vec[j - 1]->right);
                }
                else
                {
                    std::cout << empty;
                    newVec.push_back(NULL);
                    newVec.push_back(NULL);
                }
            }

            int spaces = vec.size() * 2 == newVec.size() ? prob : prevProb;
            for (int k = 0; k < spaces; k++)
                cout << empty;

            i += prevProb + 1;
        } while (i < maxH);

        cout << '\n';
        PrintLevel(newVec, lev + 1, h, prob);
    }

public:
    BTree()
    {
        root = nullptr;
    }

    BNode<T>* GetRoot()
    {
        return this->root;
    }


    void Insert(T val, int k) {
        BNode<T>* node = root;
        BNode<T>* parent = nullptr;

        if (root == nullptr)
        {
            root = new BNode<T>(val, k, NULL, NULL);
            return;
        }

        while (node != nullptr) {
            parent = node;
            node = k >= node->key ? node->right : node->left;
        }

        if (k >= parent->key)
            parent->right = new BNode<T>(val, k, NULL, NULL);
        else parent->left = new BNode<T>(val, k, NULL, NULL);
    }



    BNode<T>* Search(int key)
    {
        BNode<T>* node = root;

        while (node->key != key)
        {
            if (node->key < key)
                node = node->right;
            else
                node = node->left;

            if (node == NULL)
                return NULL;
        }

        return node;
    }

    bool Delete(int key)
    {
        BNode<T>* del = root;
        BNode<T>* parent = NULL;

        while (del->key != key)
        {
            parent = del;
            if (del->key < key)
                del = del->right;
            else
                del = del->left;

            if (del == NULL)
                return false;
        }

        // delete root
        if (del == root)
        {
            if (root->left == NULL || root->right == NULL)
            {
                root = root->right != NULL ? root->right : root->left;
                return true;
            }
        }

        // delete 0 childs
        if (del->right == NULL && del->left == NULL)
        {
            if (parent->left == del)
                parent->left = NULL;
            else parent->right = NULL;
            return true;
        }

        // delete 1 child
        if (del->left != NULL && del->right == NULL)
        {
            if (parent->left == del)
                parent->left = del->left;
            else parent->right = del->left;
            return true;
        }
        // delete 1 child
        if (del->left == NULL && del->right != NULL)
        {
            if (parent->left == del)
                parent->left = del->right;
            else parent->right = del->right;
            return true;
        }

        // delete 2 childs
        BNode<T>* rMin = del->right;
        BNode<T>* rMinParent = del->right->left ? del->right : del;
        while (rMin->left != NULL)
        {
            rMinParent = rMin;
            rMin = rMin->left;
        }

        if (rMinParent == del)
            del->right = del->right->right;
        else
            rMinParent->left = rMin->right;

        del->key = rMin->key;
        del->value = rMin->value;
        
        return true;
    }

    void PrintTree()
    {
        int h = GetHeight(root, 0);
        if (h == 0)
        {
            cout << "Дерево пустое.";
            return;
        }

        vector<BNode<T>*> first;
        first.push_back(root);
        PrintLevel(first, 1, h, pow(2, h) - 1);
    }
};



enum Color { Black, Red };

template <class T>
struct Node
{
    T value;
    int key;
    Color color;
    Node* left, * right, * parent;

    Node(T val, int k, Color c, Node* p, Node* l, Node* r)
    {
        value = val;
        key = k;
        color = c;
        parent = p;
        left = l;
        right = r;
    }

};


template <typename T>
class RBTree
{
private:
    Node<T>* root;
    int size = 0, rotates = 0;

    void RotateLeft(Node<T>* x) {
        Node<T>* y = x->right;
        x->right = y->left;
        if (y->left != NULL)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent != NULL)
        {
            if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
        }
        else
            root = y;

        y->left = x;
        x->parent = y;
    }

    void RotateRight(Node<T>* y) {
        Node<T>* x = y->left;
        y->left = x->right;
        if (x->right != NULL)
            x->right->parent = y;

        x->parent = y->parent;
        if (y->parent != NULL)
        {
            if (y == y->parent->right)
                y->parent->right = x;
            else
                y->parent->left = x;
        }
        else
            root = x;

        x->right = y;
        y->parent = x;

    }

    void ChangeNodes(Node<T>* x, Node<T>* y) {
        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        if (y != NULL)
            y->parent = x->parent;
    }

    Node<T>* FindRightMinimum(Node<T>* x) {
        while (x->left != NULL)
            x = x->left;
        return x;
    }


    void CheckBalanceAfterInsert(Node<T>* node)
    {
        Node<T>* parent = node->parent;

        while (node != root && parent->color == Red)
        {
            Node<T>* grandparent = parent->parent;

            if (parent == grandparent->right)
            {
                Node<T>* uncle = grandparent->left;

                if (uncle != NULL && uncle->color == Red)
                {
                    grandparent->color = Red;
                    parent->color = uncle->color = Black;

                    node = grandparent;
                    parent = node->parent;
                    continue;
                }

                if (parent->left == node)
                {
                    rotates++;
                    RotateRight(parent);
                    swap(node, parent);
                }

                rotates++;
                RotateLeft(grandparent);
                parent->color = Black;
                grandparent->color = Red;
                break;
            }
            else
            {
                Node<T>* uncle = grandparent->right;

                if (uncle != NULL && uncle->color == Red)
                {
                    grandparent->color = Red;
                    parent->color = uncle->color = Black;

                    node = grandparent;
                    parent = node->parent;
                    continue;
                }

                if (parent->right == node)
                {
                    rotates++;
                    RotateLeft(parent);
                    swap(node, parent);
                }

                rotates++;
                RotateRight(grandparent);
                grandparent->color = Red;
                parent->color = Black;
                break;
            }
        }
        root->color = Black;
    }

    void CheckBalanceAfterDelete(Node<T>* x)
    {
        if (x == NULL)
            return;

        while (x != root && x->color == Black)
        {
            if (x == x->parent->left)
            {
                Node<T>* neighbour = x->parent->right;
                if (neighbour->color == Red)
                {
                    neighbour->color = Black;
                    x->parent->color = Red;

                    RotateLeft(x->parent);
                    neighbour = x->parent->right;
                }

                if (neighbour->left->color == Black && neighbour->right->color == Black)
                {
                    neighbour->color = Red;
                    x = x->parent;
                }
                else
                {
                    if (neighbour->right->color == Black)
                    {
                        neighbour->left->color = Black;
                        neighbour->color = Red;

                        RotateRight(neighbour);
                        neighbour = x->parent->right;
                    }
                    neighbour->color = x->parent->color;
                    neighbour->right->color = Black;
                    x->parent->color = Black;

                    RotateLeft(x->parent);
                    x = root;
                }
            }
            else
            {
                Node<T>* neighbour = x->parent->left;
                if (neighbour->color == Red)
                {
                    neighbour->color = Black;
                    x->parent->color = Red;

                    RotateRight(x->parent);
                    neighbour = x->parent->left;
                }

                if (neighbour->right->color == Black && neighbour->left->color == Black)
                {
                    neighbour->color = Red;
                    x = x->parent;
                }
                else
                {
                    if (neighbour->left->color == Black)
                    {
                        neighbour->right->color = Black;
                        neighbour->color = Red;

                        RotateLeft(neighbour);
                        neighbour = x->parent->left;
                    }

                    neighbour->color = x->parent->color;
                    neighbour->left->color = Black;
                    x->parent->color = Black;

                    RotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = Black;
    }


    int GetHeight(Node<T>* node, int h)
    {
        if (node == NULL)
            return h;

        int leftH = GetHeight(node->left, h + 1);
        int rightH = GetHeight(node->right, h + 1);
        return rightH > leftH ? rightH : leftH;
    }

    void PrintLevel(vector<Node<T>*> vec, int lev, int h, int prevProb)
    {
        if (lev == h + 1)
            return;

        string empty = "  ";
        vector<Node<T>*> newVec;
        int prob = prevProb / 2, maxH = pow(2, h) - 1, j = 0, i = 0;

        for (; i < prob; i++)
            std::cout << empty;

        i = prob;
        do
        {
            if (j < vec.size())
            {
                if (vec[j++] != NULL)
                {
                    if (vec[j - 1]->color == Color::Red)
                        cout << "\x1b[1m\x1b[31m" << vec[j - 1]->key << "\x1b[0m";
                    else cout << "\x1b[1m\x1b[37m" << vec[j - 1]->key << "\x1b[0m";

                    newVec.push_back(vec[j - 1]->left);
                    newVec.push_back(vec[j - 1]->right);
                }
                else
                {
                    std::cout << empty;
                    newVec.push_back(NULL);
                    newVec.push_back(NULL);
                }
            }

            int spaces = vec.size() * 2 == newVec.size() ? prob : prevProb;
            for (int k = 0; k < spaces; k++)
                cout << empty;

            i += prevProb + 1;
        } while (i < maxH);

        cout << '\n';
        PrintLevel(newVec, lev + 1, h, prob);
    }

public:
    RBTree()
    {
        root = nullptr;
    }


    Node<T>* Search(Node<T>* node, int key)
    {
        if (node == NULL || node->key == key)
            return node;

        return key > node->key ? Search(node->right, key) : Search(node->left, key);
    }

    void Insert(T val, int key)
    {
        Node<T>* node = new Node<T>(val, key, Red, NULL, NULL, NULL);
        Node<T>* parent = NULL, * ins = root;

        while (ins != NULL)
        {
            parent = ins;
            ins = node->key > ins->key ? ins->right : ins->left;
        }

        if (parent != NULL)
        {
            node->parent = parent;
            if (node->key > parent->key)
                parent->right = node;
            else
                parent->left = node;
        }
        else
            root = node;

        node->color = Color::Red;
        CheckBalanceAfterInsert(node);
        size++;
    }

    bool Delete(int key)
    {
        Node<T>* del = root;

        while (del->key != key)
        {
            if (del->key < key)
                del = del->right;
            else
                del = del->left;

            if (del == NULL)
                return false;
        }

        Node<T>* child = del, * sib;
        auto ycolor = child->color;

        if (del->left == NULL)
        {
            sib = del->right;
            ChangeNodes(del, del->right);
        }
        else if (del->right == NULL)
        {
            sib = del->left;
            ChangeNodes(del, del->left);
        }
        else
        {
            child = FindRightMinimum(del->right);
            ycolor = child->color;
            sib = child->right;

            if (child->parent == del)
            {
                if (sib != NULL)
                    sib->parent = del;
            }
            else
            {
                ChangeNodes(child, child->right);
                child->right = del->right;
                child->right->parent = child;
            }

            ChangeNodes(del, child);
            child->left = del->left;
            child->left->parent = child;
            child->color = del->color;
        }
        if (ycolor == Black)
            CheckBalanceAfterDelete(sib);
        size--;
        return true;
    }

    void PrintTree()
    {
        int h = GetHeight(root, 0);
        if (h == 0)
        {
            cout << "Дерево пустое.";
            return;
        }

        vector<Node<T>*> first;
        first.push_back(root);
        PrintLevel(first, 1, h, pow(2, h) - 1);
    }


    int GetRotates()
    {
        return this->rotates;
    }

    int GetSize()
    {
        return this->size;
    }

    Node<T>* GetRoot()
    {
        return this->root;
    }
};




string GenerateProduct()
{
    // Генерация строки на основе генератора случайных чисел
    string product = "";
    int count = 1 + rand() % 6;

    for (int i = 0; i < count; i++)
        product.push_back(char(48 + rand() % 10));
    product.push_back(' ');

    count = 5 + rand() % 10;
    product.push_back(char(65 + rand() % 25));
    for (int i = 0; i < count; i++)
        product.push_back(char(97 + rand() % 25));

    return product;
}

vector<Product> GenerateProducts(int n, vector<Product> products, FileWorker fw)
{
    products.clear();
    // Генерация случайных значений
    for (int i = 0; i < n; i++)
        products.push_back(Product(GenerateProduct()));

    // Сохранение и чтение из бинарного файла
    fw.Save(products);
    return fw.Load();
}


void TestFileWorker(int size, FileWorker fw)
{
    auto begin = chrono::steady_clock::now();
    fw.Read(1).Print();
    auto end1 = chrono::steady_clock::now();
    fw.Read(size / 2).Print();
    auto end2 = chrono::steady_clock::now();
    fw.Read(size).Print();
    auto end3 = chrono::steady_clock::now();
    cout << "Время работы FW:\n";
    cout << "Начало файла: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
    cout << "Середина файла: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
    cout << "Конец файла: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";
}

void TestBinTree(int size, vector<Product> products)
{
    int bcode = products[0].code;
    int mcode = products[size / 2].code;
    int ecode = products[size - 1].code;
    auto begin = chrono::steady_clock::now();

    BTree<Product> tree;
    for (int i = 0; i < size; i++)
        tree.Insert(products[i], products[i].code);

    tree.Search(bcode)->value.Print();
    auto end1 = chrono::steady_clock::now();
    tree.Search(mcode)->value.Print();
    auto end2 = chrono::steady_clock::now();
    tree.Search(ecode)->value.Print();
    auto end3 = chrono::steady_clock::now();
    
    cout << "Время работы бинарного дерева:\n";
    cout << "Начало файла: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
    cout << "Середина файла: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
    cout << "Конец файла: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";
}

void TestRBTree(int size, vector<Product> products)
{
    int bcode = products[0].code;
    int mcode = products[size / 2].code;
    int ecode = products[size - 1].code;
    auto begin = chrono::steady_clock::now();

    RBTree<Product> tree;
    for (int i = 0; i < size; i++)
        tree.Insert(products[i], products[i].code);

    tree.Search(tree.GetRoot(), bcode)->value.Print();
    auto end1 = chrono::steady_clock::now();
    tree.Search(tree.GetRoot(), mcode)->value.Print();
    auto end2 = chrono::steady_clock::now();
    tree.Search(tree.GetRoot(), ecode)->value.Print();
    auto end3 = chrono::steady_clock::now();

    cout << "Время работы красно-черного дерева:\n";
    cout << "Начало файла: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
    cout << "Середина файла: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
    cout << "Конец файла: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";
}


void TestTime()
{
    vector<Product> products;
    FileWorker fw;

    products = GenerateProducts(testSize, products, fw);

    cout << "Замеры времени для поиска по " << testSize << " элементам:\n\n";
    TestFileWorker(testSize, fw);
    TestBinTree(testSize, products);
    TestRBTree(testSize, products);
    system("pause");
}




void WorkWithBinaryTree(vector<Product> products)
{
    BTree<Product> tree;
    for (auto p : products)
        tree.Insert(p, p.code);

    cout << "На основе данных успешно построено следующее бинарное дерево:\n";
    tree.PrintTree();
    system("pause");

    while (true)
    {
        system("cls");
        cout << "Выберите действие:\n";
        cout << "1) Вывести бинарное дерево (вывод кодов товаров)\n";
        cout << "2) Вставить товар (ввод с консоли)\n";
        cout << "3) Удалить товар по ключу (коду)\n";
        cout << "4) Найти товар по его ключу (коду)\n";
        cout << "5) Выход\n";

        int num, code;
        string s;
        Product product;
        BNode<Product>* node;
        cin >> num;
        system("cls");

        switch (num)
        {
        case 1:
            tree.PrintTree();
            system("pause");
            break;
        case 2:
            cout << "Введите данные о товарев в формате \"Код Название\": ";
            getline(cin, s);
            getline(cin, s);

            product = Product(s);
            tree.Insert(product, product.code);
            cout << "Товар успешно вставлен в дерево.\n";
            system("pause");
            break;
        case 3:
            cout << "Введите код товара для удаления: ";
            cin >> code;

            if (tree.Delete(code))
                cout << "Товар с кодом " << code << " успешно удален из дерева.\n";
            else
                cout << "Товара с кодом " << code << " не найдено в дереве.\n";;
            system("pause");
            break;
        case 4:
            cout << "Введите код товара для поиска: ";
            cin >> code;
            node = tree.Search(code);

            if (node != NULL)
                cout << "Товар с кодом " << code << " успешно найден:\nКод товара: " << node->key << ", название: " << node->value.name << "\n";
            else
                cout << "Товара с кодом " << code << " не найдено в дереве.\n";;
            system("pause");
            break;
        case 5:
            return;
        default:
            cout << "Неизвестная команда.\n";
            system("pause");
            break;
        }
        //system("pause");
    }
}

void WorkWithRedBlackTree(vector<Product> products)
{
    RBTree<Product> tree;
    for (auto p : products)
        tree.Insert(p, p.code);

    cout << "На основе данных успешно построено следующее красно-черное дерево:\n";
    tree.PrintTree();
    cout << "\nЧисло поворотов: " << tree.GetRotates() << ", число узлов: " << tree.GetSize() <<
        "; Среднее число поворотов: " << (double)tree.GetRotates() / tree.GetSize() << "\n";
    system("pause");

    while (true)
    {
        system("cls");
        cout << "Выберите действие:\n";
        cout << "1) Вывести красно-черное дерево (вывод кодов товаров)\n";
        cout << "2) Вставить товар (ввод с консоли)\n";
        cout << "3) Удалить товар по ключу (коду)\n";
        cout << "4) Найти товар по его ключу (коду)\n";
        cout << "5) Выход\n";

        int num, code;
        string s;
        Product product;
        Node<Product>* node;
        //getline(cin, s);
        cin >> num;
        system("cls");

        switch (num)
        {
        case 1:
            tree.PrintTree();
            system("pause");
            break;
        case 2:
            cout << "Введите данные о товарев в формате \"Код Название\": ";
            getline(cin, s);
            getline(cin, s);

            product = Product(s);
            tree.Insert(product, product.code);
            cout << "Товар успешно вставлен в дерево.\n";
            system("pause");
            break;
        case 3:
            cout << "Введите код товара для удаления: ";
            cin >> code;

            if (tree.Delete(code))
                cout << "Товар с кодом " << code << " успешно удален из дерева.\n";
            else
                cout << "Товара с кодом " << code << " не найдено в дереве.\n";;
            system("pause");
            break;
        case 4:
            cout << "Введите код товара для поиска: ";
            cin >> code;
            node = tree.Search(tree.GetRoot(), code);

            if (node != NULL)
                cout << "Товар с кодом " << code << " успешно найден:\nКод товара: " << node->key << ", название: "
                << node->value.name << ", В дереве эта вершина " << (node->color == Red ? "красная\n" : "черная\n");
            else
                cout << "Товара с кодом " << code << " не найдено в дереве.\n";;
            system("pause");
            break;
        case 5:
            return;
        default:
            cout << "Неизвестная команда.\n";
            system("pause");
            break;
        }
        //system("pause");
    }
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    if (isTestingTime)
    {
        testSize = 1000;
        TestTime();
        testSize = 10000;
        TestTime();
        testSize = 100000;
        TestTime();
    }


    vector<Product> products;

    cout << "Для работы с программой будет считан файл txtfile.txt, его данные запишутся в бинарный файл binfile.txt\n";
    system("pause");

    FileWorker fw;
    products = fw.Create();

    cout << "Данные успешно считаны.\n";
    system("pause");
    system("cls");


    cout << "Для работы с бинарным деревом введите 1, для работы с красно-черным введите любой другой символ: ";
    string str;
    cin >> str;

    if (str == "1")
        WorkWithBinaryTree(products);
    else
        WorkWithRedBlackTree(products);

    return 0;
}

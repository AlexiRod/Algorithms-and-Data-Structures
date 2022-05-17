#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct Abonement
{
    int number;
    char name[30] = "";
    char adress[25] = "";

    Abonement()
    {
        number = 10000000 + rand() % 90000000;
        int nameLength = 5 + rand() % 20;
        int adressLength = 9 + rand() % 15;

        name[0] = char(65 + rand() % 25);
        adress[0] = char(65 + rand() % 25);

        for (int i = 0; i < nameLength; i++)
            name[i] = char(97 + rand() % 25);
        for (int i = 0; i < adressLength; i++)
            adress[i] = char(97 + rand() % 25);
    }

    Abonement(int num, string n, string a)
    {
        number = num;

        for (int i = 0; i < n.size(); i++)
            name[i] = n[i];

        for (int i = 0; i < a.size(); i++)
            adress[i] = a[i];
    }

    vector<Abonement> GenerateAbonements(int n)
    {
        vector<Abonement> abonements;
        for (int i = 0; i < n; i++)
            abonements.push_back(Abonement());
        return abonements;
    }
};

template <typename T>
struct Node
{
    T value;
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k, T val)
    {
        left = nullptr;
        right = nullptr;

        height = 1;
        value = val;
        key = k;
    }

    string ToString()
    {
        Abonement a = (Abonement)value;
        return "Номер " + to_string(a.number) + ", ФИО " + a.name + ", Адрес " + a.adress;
    }
};



template <typename T>
class BinaryTree
{
public:

    BinaryTree()
    {
        space = "  ";
        root = nullptr;
    }

    void Insert(int key, T val) {
        if (!root)
        {
            root = new Node<T>(key, val);
            return;
        }

        Node<T>* node = root;
        while (node)
        {
            if (key >= node->key)
            {
                if (!node->right)
                {
                    node->right = new Node<T>(key, val);
                    break;
                }
                node = node->right;
            }
            else
            {
                if (!node->left)
                {
                    node->left = new Node<T>(key, val);
                    break;
                }
                node = node->left;
            }
        }
    }

    bool Delete(int key)
    {
        if (!root)
            return false;

        Node<T>* node = root;
        Node<T>* parent = root;

        while (node->key != key)
        {
            parent = node;
            node = node->key < key ? node->right : node->left;
            if (!node)
                return false;
        }


        if (!node->right && !node->left)
        {
            if (node == root)
            {
                root = nullptr;
                return true;
            }

            if (node == parent->left)
                parent->left = nullptr;
            else
                parent->right = nullptr;

            return true;
        }

        if (node->left && !node->right)
        {
            if (node == root)
            {
                root = root->left;
                return true;
            }

            if (parent->left == node)
                parent->left = node->left;
            else parent->right = node->left;

            return true;
        }
        if (!node->left && node->right)
        {
            if (node == root)
            {
                root = root->right;
                return true;
            }

            if (parent->left == node)
                parent->left = node->right;
            else parent->right = node->right;

            return true;
        }

        Node<T>* rightMinimum = node->right;
        Node<T>* rightMinimumParent = node->right->left ? node->right : node;
        while (rightMinimum->left)
        {
            rightMinimumParent = rightMinimum;
            rightMinimum = rightMinimum->left;
        }

        if (rightMinimum == node->right)
            node->right = node->right->right;
        else
            rightMinimumParent->left = rightMinimum->right;

        node->key = rightMinimum->key;
        node->value = rightMinimum->value;

        return true;
    }

    Node<T>* Find(int key)
    {
        Node<T>* node = root;
        while (node && node->key != key)
            node = node->key < key ? node->right : node->left;

        return node;
    }

    void Print()
    {
        vector<Node<T>*> first;
        first.push_back(root);
        PrintNodes(first, -1, GetNodeHeight(root, 0));
        cout << "\n";
    }

private:

    Node<T>* root;
    string space;

    int GetNodeHeight(Node<T>* node, int h)
    {
        return node ? max(GetNodeHeight(node->left, h + 1), GetNodeHeight(node->right, h + 1)) : h;
    }

    void PrintNodes(vector<Node<T>*> nodes, int lastOffset, int counter)
    {
        if (counter == 0)
            return;

        if (lastOffset == -1)
            lastOffset = pow(2, counter - 1) - 1;

        int offset = pow(2, counter - 1) - 1;
        for (int i = 0; i < offset; i++)
            cout << space;

        vector<Node<T>*> nextNodes;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i])
            {
                cout << nodes[i]->key;
                nextNodes.push_back(nodes[i]->left);
                nextNodes.push_back(nodes[i]->right);
            }
            else
            {
                cout << space;
                nextNodes.push_back(nullptr);
                nextNodes.push_back(nullptr);
            }

            if (i != nodes.size() - 1)
                for (int j = 0; j < lastOffset; j++)
                    cout << space;
        }

        for (int i = 0; i < offset; i++)
            cout << space;
        cout << '\n';

        PrintNodes(nextNodes, offset, --counter);
    }
};



template <typename T>
class AVLTree
{
public:

    AVLTree()
    {
        space = "  ";
        root = nullptr;
        rotates = 0;
    }

    void PrintRotatesInfo(int n)
    {
        cout << "Всего ключей: " << n << ",\nВыполнено поворотов: " << rotates << "\nСреднее число поворотов: " << (double)rotates / n << "\n";
    }

    void Insert(int key, T value)
    {
        Node<T>* ins = new Node<T>(key, value);
        root = root ? BalancedInsert(root, ins) : ins;
        FixHeight(root);
    }

    bool Delete(int key)
    {
        if (!Find(key))
            return false;

        root = BalancedDelete(root, key);
        return true;
    }

    Node<T>* Find(int k)
    {
        Node<T>* node = root;
        while (node && node->key != k)
            node = node->key < k ? node->right : node->left;

        return node;
    }

    void Print()
    {
        vector<Node<T>*> first;
        first.push_back(root);
        PrintNodes(first, -1, GetNodeHeight(root));
        cout << "\n";
    }

private:

    int rotates;
    string space;
    Node<T>* root;


    int GetHeight(Node<T>* node)
    {
        return node ? node->height : 0;
    }

    int GetBalance(Node<T>* node)
    {
        return GetHeight(node->right) - GetHeight(node->left);
    }

    int GetNodeHeight(Node<T>* node)
    {
        return node ? max(GetNodeHeight(node->right) + 1, GetNodeHeight(node->left) + 1) : 0;
        //return node ? (GetBalance(node) > 0 ? GetNodeHeight(node->right) + 1 : GetNodeHeight(node->left) + 1) : 0;
    }

    void FixHeight(Node<T>* node)
    {
        node->height = max(GetHeight(node->left), GetHeight(node->right)) + 1;
    }


    Node<T>* RotateRight(Node<T>* node)
    {
        Node<T>* left = node->left;
        node->left = left->right;
        left->right = node;

        rotates++;
        FixHeight(left);
        FixHeight(node);
        return left;
    }

    Node<T>* RotateLeft(Node<T>* node)
    {
        Node<T>* right = node->right;
        node->right = right->left;
        right->left = node;

        rotates++;
        FixHeight(right);
        FixHeight(node);
        return right;
    }

    Node<T>* RemoveRightMinimum(Node<T>* node, Node<T>* rightMinimum)
    {
        if (node == rightMinimum)
            return rightMinimum->right;

        node->left = RemoveRightMinimum(node->left, rightMinimum);
        return GetBalancedNode(node);
    }

    Node<T>* GetBalancedNode(Node<T>* node)
    {
        FixHeight(node);
        int balanceFactor = GetBalance(node);

        if (balanceFactor == -2)
        {
            if (GetHeight(node->left->left) < GetHeight(node->left->right))
                node->left = RotateLeft(node->left);
            return RotateRight(node);
        }

        if (balanceFactor == 2)
        {
            if (GetHeight(node->right->left) > GetHeight(node->right->right))
                node->right = RotateRight(node->right);
            return RotateLeft(node);
        }

        return node;
    }

    Node<T>* BalancedInsert(Node<T>* node, Node<T>* ins)
    {
        if (!node)
            return ins;

        if (node->key <= ins->key)
            node->right = BalancedInsert(node->key > ins->key ? node->left : node->right, ins);
        else
            node->left = BalancedInsert(node->key > ins->key ? node->left : node->right, ins);

        return GetBalancedNode(node);
    }

    Node<T>* BalancedDelete(Node<T>* node, int key)
    {
        if (!node)
            return nullptr;

        if (node->key == key)
        {
            if (!node->left && !node->right)
                return nullptr;

            if (node->left && !node->right)
                return node->left;


            Node<T>* rightMinimum = node->right;
            while (rightMinimum->left)
                rightMinimum = rightMinimum->left;

            node->value = rightMinimum->value;
            node->key = rightMinimum->key;
            node->right = RemoveRightMinimum(node->right, rightMinimum);

            return GetBalancedNode(node);
        }


        if (node->key <= key)
            node->right = BalancedDelete(node->right, key);
        else
            node->left = BalancedDelete(node->left, key);

        return GetBalancedNode(node);
    }

    void PrintNodes(vector<Node<T>*> nodes, int lastOffset, int counter)
    {
        if (counter == 0)
            return;

        if (lastOffset == -1)
            lastOffset = pow(2, counter - 1) - 1;

        int offset = pow(2, counter - 1) - 1;
        for (int i = 0; i < offset; i++)
            cout << space;

        vector<Node<T>*> nextNodes;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i])
            {
                cout << nodes[i]->key;
                nextNodes.push_back(nodes[i]->left);
                nextNodes.push_back(nodes[i]->right);
            }
            else
            {
                cout << space;
                nextNodes.push_back(nullptr);
                nextNodes.push_back(nullptr);
            }

            if (i != nodes.size() - 1)
                for (int j = 0; j < lastOffset; j++)
                    cout << space;
        }

        for (int i = 0; i < offset; i++)
            cout << space;
        cout << '\n';

        PrintNodes(nextNodes, offset, --counter);
    }
};

class FilesManager
{
public:

    vector<Abonement> ReadFromText(string path)
    {
        string s;
        vector<Abonement> abonements;

        ifstream file(path);
        while (!file.eof())
        {
            getline(file, s);
            stringstream stream(s);
            string value, name, adress;
            stream >> value >> name >> adress;
            abonements.push_back(Abonement(stoi(value), name, adress));
        }

        file.close();
        return abonements;
    }

    vector<Abonement> ReadFromBinary(string path)
    {
        Abonement a;
        vector<Abonement> abonements;

        ifstream file(path, ios::binary | ios::in);
        file.read((char*)&a, sizeof(Abonement));
        while (!file.eof())
        {
            file.read((char*)&a, sizeof(Abonement));
            abonements.push_back(a);
        }
        file.close();
        abonements.pop_back();

        return abonements;
    }

    void SaveToBinary(vector<Abonement> abonements, string path)
    {
        ofstream file;
        Abonement empty;

        file.open(path, ios::out);
        file.write((char*)&empty, sizeof(Abonement));
        file.close();

        file.open(path, ios::binary | ios::in | ios::app);
        for (int i = 0; i < abonements.size(); i++)
            file.write((char*)&abonements[i], sizeof(Abonement));
        file.close();
    }

    void AddToBinary(Abonement abonement, vector<Abonement> abonements, string path)
    {
        abonements.push_back(abonement);
        SaveToBinary(abonements, path);
    }

    Abonement LinearSearchFomBinary(int index, string path)
    {
        int i = 0;
        while (i != index)
            i++;

        return ReadFromBinary(path)[i];
    }
};

class TreeInterface
{
public:

    TreeInterface()
    {
        cout << "Выберите вариант работы программы:\n";
        cout << "1. Работа с бинарным деревом\n";
        cout << "2. Работа с АВЛ-деревом\n";
        cout << "3. Тестирование времени поиска\n";
        int number;
        cin >> number;

        switch (number)
        {
        case 1:
            BinaryTreeInterface();
            break;
        case 2:
            AVLTreeInterface();
            break;
        case 3:
            SearchTimeInterface();
            break;
        default:
            cout << "Неправильный номер";
            break;
        }
    }

    void BinaryTreeInterface()
    {
        FilesManager manager;
        BinaryTree<Abonement> tree;
        vector<Abonement> abonements = manager.ReadFromText("abonements.txt");
        for (int i = 0; i < abonements.size(); i++)
            tree.Insert(abonements[i].number, abonements[i]);

        system("cls");
        cout << "Дерево, построенное из исходных данных:\n";
        tree.Print();
        system("pause");

        while (true)
        {
            system("cls");
            cout << "Выберите операцию:\n";
            cout << "1. Вставка в дерево\n";
            cout << "2. Удаление из дерева\n";
            cout << "3. Поиск в дереве\n";
            cout << "4. Вывод дерева в консоль\n";

            int number;
            string name, adress;
            Node<Abonement>* node;

            cin >> number;
            system("cls");

            switch (number)
            {
            case 1:
                cout << "Введите номер абонемента: ";
                cin >> number;
                cout << "Введите ФИО владельца абонемента: ";
                cin >> name;
                cout << "Введите адрес владельца абонемента: ";
                cin >> adress;

                tree.Insert(number, Abonement(number, name, adress));
                cout << "Вставка завершена.\n";
                break;
            case 2:
                cout << "Введите номер абонемента: ";
                cin >> number;

                if (tree.Delete(number))
                    cout << "Абонемент удален. Удаление завершено\n";
                else
                    cout << "В дереве не найдено абонемента с таким номером.\n";
                break;
            case 3:
                cout << "Введите номер абонемента: ";
                cin >> number;
                node = tree.Find(number);

                if (node)
                    cout << "Найденный в дереве абонемент:\n" << node->ToString() << "\n";
                else
                    cout << "В дереве не найдено абонемента с таким номером.\n";;
                break;
            case 4:
                cout << "Бинарное дерево:\n";
                tree.Print();
                break;
            }
            system("pause");
        }
    }

    void AVLTreeInterface()
    {
        FilesManager manager;
        AVLTree<Abonement> tree;
        vector<Abonement> abonements = manager.ReadFromText("abonements.txt");
        for (int i = 0; i < abonements.size(); i++)
            tree.Insert(abonements[i].number, abonements[i]);
        
        system("cls");
        cout << "АВЛ-дерево, построенное из исходных данных:\n";
        tree.Print();
        tree.PrintRotatesInfo(abonements.size());
        system("pause");

        while (true)
        {
            system("cls");
            cout << "Выберите операцию:\n";
            cout << "1. Вставка в АВЛ-дерево\n";
            cout << "2. Удаление из АВЛ-дерева\n";
            cout << "3. Поиск в АВЛ-дереве\n";
            cout << "4. Вывод АВЛ-дерева в консоль\n";

            int number;
            string name, adress;
            Node<Abonement>* node;

            cin >> number;
            system("cls");

            switch (number)
            {
            case 1:
                cout << "Введите номер абонемента: ";
                cin >> number;
                cout << "Введите ФИО владельца абонемента: ";
                cin >> name;
                cout << "Введите адрес владельца абонемента: ";
                cin >> adress;

                tree.Insert(number, Abonement(number, name, adress));
                cout << "Вставка завершена.\n";
                break;
            case 2:
                cout << "Введите номер абонемента: ";
                cin >> number;

                if (tree.Delete(number))
                    cout << "Абонемент удален. Удаление завершено\n";
                else
                    cout << "В АВЛ-дереве не найдено абонемента с таким номером.\n";
                break;
            case 3:
                cout << "Введите номер абонемента: ";
                cin >> number;
                node = tree.Find(number);

                if (node)
                    cout << "Найденный в АВЛ-дереве абонемент:\n" << node->ToString() << "\n";
                else
                    cout << "В АВЛ-дереве не найдено абонемента с таким номером.\n";;
                break;
            case 4:
                cout << "АВЛ-дерево:\n";
                tree.Print();
                break;
            }
            system("pause");
        }
    }
    
    void SearchTimeInterface()
    {
        int n = 10000;
        cout << "Результаты для n = " << n << ":\n";
        Abonement a;
        BinaryTree<Abonement> bintree;
        AVLTree<Abonement> avltree;
        FilesManager manager;

        vector<Abonement> abonements = a.GenerateAbonements(n);
        for (int i = 0; i < abonements.size(); i++)
        {
            bintree.Insert(abonements[i].number, abonements[i]);
            avltree.Insert(abonements[i].number, abonements[i]);
        }
        manager.SaveToBinary(abonements, "abonements.bin");

        Abonement first = abonements[0];
        Abonement middle = abonements[n / 2];
        Abonement last = abonements[n - 1];
        Node<Abonement>* node;

        cout << "Поиск в бинарном дереве:\n";
        auto begin = chrono::steady_clock::now();
        node = bintree.Find(first.number);
        auto end1 = chrono::steady_clock::now();
        node = bintree.Find(middle.number);
        auto end2 = chrono::steady_clock::now();
        node = bintree.Find(last.number);
        auto end3 = chrono::steady_clock::now();
        cout << "Первая запись: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
        cout << "Запись из середины: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
        cout << "Последняя запись: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";

        cout << "Поиск в АВЛ-дереве:\n";
        begin = chrono::steady_clock::now();
        node = avltree.Find(first.number);
        end1 = chrono::steady_clock::now();
        node = avltree.Find(middle.number);
        end2 = chrono::steady_clock::now();
        node = avltree.Find(last.number);
        end3 = chrono::steady_clock::now();
        cout << "Первая запись: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
        cout << "Запись из середины: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
        cout << "Последняя запись: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";

        cout << "Линейный поиск в файле:\n";
        begin = chrono::steady_clock::now();
        a = manager.LinearSearchFomBinary(0, "abonements.bin");
        end1 = chrono::steady_clock::now();
        a = manager.LinearSearchFomBinary(n / 2, "abonements.bin");
        end2 = chrono::steady_clock::now();
        a = manager.LinearSearchFomBinary(n - 1, "abonements.bin");
        end3 = chrono::steady_clock::now();
        cout << "Первая запись: " << chrono::duration_cast<chrono::nanoseconds>(end1 - begin).count() << " нс\n";
        cout << "Запись из середины: " << chrono::duration_cast<chrono::nanoseconds>(end2 - begin).count() << " нс\n";
        cout << "Последняя запись: " << chrono::duration_cast<chrono::nanoseconds>(end3 - begin).count() << " нс\n\n";

        system("pause");
    }
};

int main()
{
    setlocale(LC_ALL, "");
    TreeInterface treeint;
}
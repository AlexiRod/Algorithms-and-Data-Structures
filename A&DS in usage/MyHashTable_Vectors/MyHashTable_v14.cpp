#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <Windows.h>

using namespace std;



struct city
{
    char code[15];
    char name[25];
};

city StringToCity(string s)
{
    city newCity;

    for (int i = 0; i < 15; i++)
        newCity.code[i] = '\0';
    for (int i = 0; i < 25; i++)
        newCity.name[i] = '\0';

    int half = 0;
    for (int i = 0, part = 1; i < s.size(); i++)
    {
        if (s[i] == ' ')
        {
            part = 2;
            half = i + 1;
            continue;
        }
        if (part == 1)
            newCity.code[i] = s[i];
        else
            newCity.name[i - half] = s[i];
    }

    return newCity;
}

vector<city> ReadCitiesFromFile(ifstream& fb)
{
    city x;
    vector<city> cities;

    // Пустой город игнорируем
    fb.read((char*)&x, sizeof(city));
    while (!fb.eof())
    {
        fb.read((char*)&x, sizeof(city));
        cities.push_back(x);
    }
    fb.close();

    // Лишний город удаляем
    cities.pop_back();
    return cities;
}

class MyHashTable
{
public:
    MyHashTable(int n, int h, bool isTest);
    void ReadTableFromFile();
    void Rehash(vector<city>& list, city a);
    int GetHash(string code);
    int GetIndex(string code);
    void Paste(city a);
    void Remove(string code);
    void SearchCity(string code);
    void PrintTable();
    void PrintList(vector<city> list);

private:
    bool isTest;
    int p = 1, n = 0;
    vector<vector<city>> cityList;
};


MyHashTable::MyHashTable(int _n, int _p, bool _isTest)
{
    n = _n;
    p = _p;
    isTest = _isTest;

    ReadTableFromFile();
}

void MyHashTable::ReadTableFromFile()
{
    // Инициализация
    vector<city> emptyVector;
    for (int i = 0; i < n; i++)
        cityList.push_back(emptyVector);

    // Чтение из файла и вставка
    ifstream fbb("data.bin", ios::in | ios::binary);
    vector<city> cities = ReadCitiesFromFile(fbb);
    for (int i = 0; i < cities.size(); i++)
        Paste(cities[i]);
}


void MyHashTable::Rehash(vector<city>& list, city a)
{
    // Рехеширование производится путем добавления ключа в конец цепочки
    list.push_back(a);
}

int MyHashTable::GetHash(string code)
{
    // Хеширование: sum [(s[i] * 10^i) % p]
    int hash = 0;
    for (int i = 0; i < code.size(); i++)
        hash += int(int(code[i]) * pow(10, i)) % p;

    return hash % n;
}

int MyHashTable::GetIndex(string code)
{
    int i = GetHash(code);
    cout << "Поиск индекса по хешу: Для кода " << code << " хеш равен " << i << "\n";

    for (int j = 0; j < cityList[i].size(); j++)
        if (cityList[i][j].code == code)
        {
            //if (isTest)
            //    cout << "Запись успешно найдена.\n";
            return i;
        }

    if (isTest)
        cout << "Цепочки, содержащей город с кодом " << code << "(хеш равен " << i << ") не найдено.\n";
    return -1;
}


void MyHashTable::Paste(city a)
{
    int i = GetHash(a.code);
    cout << "Для ключа " << a.code << " сгенерирован хеш: " << i << "\n";

    for (int j = 0; j < cityList[i].size(); j++)
        if (cityList[i][j].code == a.code)
        {
            cout << "В таблице обнаружен идентичный ключ. Дубликат не будет вставлен.\n";
            return;
        }

    if (cityList[i].size() == 0)
    {
        cityList[i].push_back(a);
        cout << "Индекс " << i << " свободен. Успешно вставлена новая цепочка с одним звеном.\n";
        return;
    }

    cout << "Индекс уже используется. Будет произведено рехеширование - в соответствующую цепочку добаится новое звено." << "\n";
    Rehash(cityList[i], a);
}

void MyHashTable::Remove(string code)
{
    auto begin = chrono::steady_clock::now();
    int i = GetIndex(code);

    if (i != -1)
    {
        for (int j = 0; j < cityList[i].size(); j++)
            if (cityList[i][j].code == code)
            {
                cityList[i].erase(cityList[i].begin() + j);
                cout << "Город с кодом " << code << " (хешируется в " << i << ") успешно удален из соответствующей цепочки.\n";
                break;
            }
    }
    else
        cout << "Удаление невозможно. Цепочки с нужным ключом не было найдено.\n";


    if (!isTest)
    {
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << "Время работы: " << elapsed.count() << " ns\n";
    }
}

void MyHashTable::SearchCity(string code)
{
    auto begin = chrono::steady_clock::now();
    int i = GetIndex(code);

    if (i != -1)
    {
        cout << "Город с кодом " << code << " успешно найден в таблице:\n";
        for (int j = 0; j < cityList[i].size(); j++)
            if (cityList[i][j].code == code)
            {
                cout << "Код города: " << code << "; Название: " << cityList[i][j].name << "\n";
                break;
            }


        if (isTest)
        {
            cout << "Цепочка, в которой был найден город (хеш-значение " << i << "):\n";
            PrintList(cityList[i]);
        }
    }
    else
        cout << "Поиск не удался. Цепочки с нужным ключом не было найдено.\n";


    if (!isTest)
    {
        auto end = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << "Время работы: " << elapsed.count() << " ns\n";
    }
}


void MyHashTable::PrintTable()
{
    for (int i = 0; i < n; i++)
        if (cityList[i].size() != 0)
        {
            cout << i << " | ";
            PrintList(cityList[i]);
        }
        else cout << i << " |\n";
}

void MyHashTable::PrintList(vector<city> list)
{
    cout << "{" << list[0].code << " : " << list[0].name << "}";
    for (int i = 1; i < list.size(); i++)
        cout << " => {" << list[i].code << " : " << list[i].name << "}";
    cout << "\n";
}




void Input()
{
    cout << "Введите 5 тестовых строк в формате \"Код_города Название_города\" (15 символов под код, 25 под название):\n";

    // Очистка файла от прошлых данных
    ofstream fclean;
    city emptyCity;
    fclean.open("data.bin", ios::out);
    fclean.write((char*)&emptyCity, sizeof(city));
    fclean.close();

    ofstream fb;
    fb.open("data.bin", ios::in | ios::app | ios::binary);

    string a;
    getline(cin, a);
    for (int i = 0; i < 5; i++)
    {
        // Ввод и запись в файл
        getline(cin, a);
        city x = StringToCity(a);
        fb.write((char*)&x, sizeof(city));
    }
    fb.close();
}

string GenerateRandomCity()
{
    // Генерация строки на основе генератора случайных чисел
    string city = "";
    int count = 1 + rand() % 8;

    for (int i = 0; i < count; i++)
        city.push_back(char(48 + rand() % 10));
    city.push_back(' ');

    count = 4 + rand() % 10;
    city.push_back(char(65 + rand() % 25));
    for (int i = 0; i < count; i++)
        city.push_back(char(97 + rand() % 25));

    return city;
}

void GenerateCities(int n)
{
    // Очистка файла от прошлых данных
    ofstream fclean;
    city emptyCity;
    fclean.open("data.bin", ios::out);
    fclean.write((char*)&emptyCity, sizeof(city));
    fclean.close();


    ofstream fb;
    fb.open("data.bin", ios::in | ios::app | ios::binary);
    for (int i = 0; i < n; i++)
    {
        // Генерация случайного города и запись в файл
        city x = StringToCity(GenerateRandomCity());
        fb.write((char*)&x, sizeof(city));
    }
    fb.close();
}

void WorkWithTable(MyHashTable ht)
{
    // Метод для работы с меню таблицы
    while (true)
    {
        int num;
        string s;
        system("cls");
        cout << "1) Найти город по его коду (ключу)\n";
        cout << "2) Удалить город по его коду (ключу)\n";
        cout << "3) Вывод всей хеш таблицы\n";
        cout << "4) Выход\n";
        cin >> num;

        switch (num)
        {
        case 1:
        {
            system("cls");
            cout << "Введите код города для поиска: ";
            cin >> s;

            ht.SearchCity(s);
            system("pause");
            break;
        }
        case 2:
        {
            system("cls");
            cout << "Введите код города для удаления: ";
            cin >> s;
         
            ht.Remove(s);
            system("pause");
            break;
        }
        case 3:
        {
            system("cls");
            cout << "Построенная хеш таблица:\n";
            
            ht.PrintTable();
            system("pause");
            break;
        }
        case 4:
        {
            system("cls");
            return;
        }
        }
    }
}



int msdain(int a)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251); 
    srand(time(0));

    cout << "Выберите способ работы с программой:\n";
    cout << "1) Заполнение данных вручную (тестировка)\n";
    cout << "2) Заполнение случайными данными (замер времени)\n";

    int num;
    cin >> num;
    system("cls");
    switch (num)
    {
    case 1:
    {
        Input();

        int p;
        cout << "Введите модуль p для хеш функции: ";
        cin >> p;
        system("cls");

        cout << "Создание хеш-таблицы:\n\n";
        MyHashTable ht(5, p, true);
        cout << "\nХеш-таблица успешно создана.\n";
        system("pause");

        WorkWithTable(ht);
        break;
    }
    case 2:
    {
        int n, p;
        cout << "Введите количество генерируемых городов: ";
        cin >> n;
        cout << "Введите модуль p для хеш функции: ";
        cin >> p;
        system("cls");

        cout << "Создание хеш-таблицы:\n\n";
        GenerateCities(n);
        MyHashTable ht(n, p, false);
        cout << "\nХеш-таблица успешно построена.\n";
        system("pause");

        WorkWithTable(ht);
        break;
    }
    }
    return 0;
}


/*

#include <iostream>
#include <vector>

using std::vector;

struct sVector {
    vector<int> indexes;
    vector<double> elements;

    sVector() {
        indexes = vector<int>();
        elements = vector<double>();
    }
};

void add(sVector* vector, int ind, double val) {
    vector->indexes.push_back(ind);
    vector->elements.push_back(val);
}

sVector* vector_multiply(sVector* a, sVector* b) {
    int i = 0;
    sVector* res = new sVector();

    if (a->indexes.size() != b->indexes.size()) {
        return nullptr;
    }

    while (i < a->indexes.size()) {
        res->indexes.push_back(i);
        res->elements.push_back(a->elements[i] + b->elements[i]);
    }

    return res;
}

int main() {
    sVector* a = new sVector();
    sVector* b = new sVector();

    for (int i = 0; i < 5; ++i)
    {
        int index;
        double val;
        std::cin >> index >> val;
        add(a, index, val);
    }
    for (int i = 0; i < 5; ++i)
    {
        int index;
        double val;
        std::cin >> index >> val;
        add(b, index, val);
    }

    std::cout << vector_multiply(a, b) << '\n';

    delete a;
    delete b;
}



template <typename T>
class HashTable {
private:
    T** table;
    int size;

    int hasher = 0;
    size_t Hash(T* a) {
        // Тут должна быть реализация, но пока будет просто возврат возрастающих значений
        return hasher++;
    }


public:
    HashTable(int n) {
        table = new T*[n];
        size = n;

        for (int i = 0; i < n; i++)
            table[i] = nullptr;
    }

    void insert(T b) {
        T* a = &b;
        int k = Hash(a) % size;

        for (int i = 0; i < size; i++) {
            if (table[(k + i) % size] == nullptr) {
                table[(k + i) % size] = a;
                return;
            }
        }
    }

    T*& find(T b)
    {
        T* a = &b;
        int k = Hash(a) % size;

        for (int i = 0; i < size; i++) {
            // Здесь должно быть корректное сравнение в зависимости от типа данных
            if (table[(k + i) % size] == a) {
                return table[(k + i) % size];
            }
        }

        T* nullp(0);
        return nullp;
    }

    bool remove(T b) {
        //получаем ссылку на указатель
        T*& a = find(b);
        if (!a)
            return false;

        //зануляем ссылку -> зануляем исходный указатель
        a = nullptr;
        return true;
    }
};

*/
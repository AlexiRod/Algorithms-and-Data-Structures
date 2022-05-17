#include <fstream>
#include <istream>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <Windows.h>

using namespace std;
const int len = 40;

struct university
{
    int code;
    char name[len];

    void InitializeFromString(string s)
    {
        for (int i = 0; i < len; i++)
            name[i] = '\0';

        string codeString = "";
        int spacePlace = -1;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ' ')
                spacePlace = i + 1;
            else if (spacePlace == -1)
                codeString.push_back(s[i]);
            else name[i - spacePlace] = s[i];
        }
        code = stoi(codeString);
    }
};


void SaveDataToFile(vector<university>& universities)
{
    ofstream file;
    university a;

    file.open("data.bin", ios::out);
    file.write((char*)&a, sizeof(university));
    file.close();
    file.open("data.bin", ios::binary | ios::in | ios::app);

    for (int i = 0; i < universities.size(); i++)
        file.write((char*)&universities[i], sizeof(university));
    file.close();
}

vector<university> LoadDataFromFile()
{
    university a;
    ifstream file("data.bin", ios::binary | ios::in);
    file.read((char*)&a, sizeof(university));

    vector<university> universities;
    while (!file.eof())
    {
        file.read((char*)&a, sizeof(university));
        universities.push_back(a);
    }
    file.close();

    universities.pop_back();
    return universities;
}

university ReadUniversityFromFile(int index)
{
    // Индексация с 1
    university a, b;
    b.code = -1;

    ifstream file("data.bin", ios::binary | ios::in);
    file.read((char*)&a, sizeof(university));

    int i = 1;
    while (!file.eof())
    {
        file.read((char*)&a, sizeof(university));
        if (i++ == index)
            return a;
    }
    return b;
}

void AddUniversirtyToFile(vector<university>& universities, university university)
{
    universities.push_back(university);
    SaveDataToFile(universities);
}

void DeleteUniversirtyDromFile(vector<university>& universities, int index)
{
    universities.erase(universities.begin() + index);
    SaveDataToFile(universities);
}


struct element
{
    int key; // = code
    university value;
    element* next = nullptr;

    element* Find(int findKey)
    {
        if (this == nullptr)
            return NULL;

        if (this->key == findKey)
            return this;

        if (this->next == nullptr)
            return NULL;

        return (this->next)->Find(findKey);
    }
    element* Find(element child)
    {
        if (this->next == nullptr)
            return NULL;

        if (this->next->key == child.key)
            return this;

        return (this->next)->Find(child);
    }
};


class HashTable
{
private:
    int n;
    vector<element*> list;
public:
    HashTable()
    {
        vector<university> universities = LoadDataFromFile();
        n = universities.size();

        for (int i = 0; i < n; i++)
            list.push_back(nullptr);

        for (int i = 0; i < n; i++)
            AddKey(universities[i].code, universities[i], false);

        if (n == 0)
        {
            list.push_back(nullptr);
            n = 1;
        }
    }


    vector<university> GetAllUniversities()
    {
        vector<university> universities;
        for (int i = 0; i < list.size(); i++)
        {
            element* cur = list[i];
            while (cur != nullptr)
            {
                universities.push_back(cur->value);
                cur = cur->next;
            }
        }
        return universities;
    }

    int GetHash(int key)
    {
        long long num, multi = 1, sum = 0, i = 1;
        while (key != 0)
        {
            num = key % 10;
            multi = (multi * num * (long long)pow(10, i++)) % INT32_MAX;
            sum += num;
            key /= 10;
        }
        if (sum == 0)
            return 0;
        return (int)abs(multi / sum) % n;
    }

    void Rehash(bool isTime)
    {
        vector<university> universities = GetAllUniversities();
        SaveDataToFile(universities);

        auto begin = chrono::steady_clock::now();
        n *= 2;
        list.clear();

        for (int i = 0; i < n; i++)
            list.push_back(nullptr);

        for (int i = 0; i < universities.size(); i++)
            AddKey(universities[i].code, universities[i], false);

        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        if (isTime)
            cout << "\nВремя работы: " << time.count() << " нс\n";
    }


    element* AddToList(university newUniversity, element& prevElement)
    {
        element* a = new element();
        a->key = newUniversity.code;
        a->value = newUniversity;
        a->next = &prevElement;

        return a;
    }

    int AddKey(int key, university newUniversity, bool isTime)
    {
        auto begin = chrono::steady_clock::now();
        int i = GetHash(key), ret = 0;

        if (list[i] == nullptr)
        {
            element* a = new element();
            a->key = key;
            a->value = newUniversity;
            a->next = nullptr;
            list[i] = a;
            ret = 1;
        }
        else
        {
            list[i] = AddToList(newUniversity, *list[i]);
            ret = 2;
        }

        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        if (isTime)
            cout << "\nВремя работы: " << time.count() << " нс\n";

        vector<university> universities = GetAllUniversities();
        SaveDataToFile(universities);
        return ret;
    }

    int DeleteKey(int key, bool isTime)
    {
        auto begin = chrono::steady_clock::now();
        int i = GetHash(key);
        if (list[i] == nullptr)
            return -1;

        // Если элемент единственный в цепочке
        if (list[i]->key == key)
        {
            element* del = list[i];
            list[i] = list[i]->next;
            delete del;

            auto end = chrono::steady_clock::now();
            auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
            if (isTime)
                cout << "\nВремя работы: " << time.count() << " нс\n";

            vector<university> universities = GetAllUniversities();
            SaveDataToFile(universities);
            return 1;
        }

        element* find = FindByKey(key, false);
        if (find == nullptr)
            return -1;

        element* parent = list[i]->Find(*find);
        parent->next = find->next;
        delete find;

        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        if (isTime)
            cout << "\nВремя работы: " << time.count() << " нс\n";

        vector<university> universities = GetAllUniversities();
        SaveDataToFile(universities);
        return 2;
    }

    element* FindByKey(int key, bool isTime)
    {
        auto begin = chrono::steady_clock::now();

        int i = GetHash(key);
        element* ret = list[i]->Find(key);

        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        if (isTime)
            cout << "\nВремя работы: " << time.count() << " нс\n";
        return ret;
    }


    void PrintTable(bool isTime)
    {
        auto begin = chrono::steady_clock::now();
        for (int i = 0; i < n; i++)
        {
            cout << i << ") ";
            PrintList(list[i]);
            cout << '\n';
        }
        cout << '\n';
        cout << '\n';
        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        if (isTime)
            cout << "\nВремя работы: " << time.count() << " нс\n";
    }

    void PrintList(element* cur)
    {
        if (cur == nullptr)
        {
            cout << '-';
            return;
        }
        cout << "[" << cur->key << "; " << cur->value.name << "]";
        if (cur->next == nullptr)
            return;

        cout << " -> ";
        PrintList(cur->next);
    }


    string GenerateStringUniversity()
    {
        string univercityString = "";

        int len = 1 + rand() % 7;
        for (int i = 0; i < len; i++)
            univercityString.push_back(char(48 + rand() % 10));
        univercityString.push_back(' ');

        if (rand() % 4 == 0) // Название состоит только из заглавных
        {
            len = 3 + rand() % 4;
            for (int i = 0; i < len; i++)
                univercityString.push_back(char(65 + rand() % 25));
        }
        else // Название - строка, начинающаяся с заглавной
        {
            len = 5 + rand() % 20;
            univercityString.push_back(char(65 + rand() % 25));
            for (int i = 0; i < len; i++)
                univercityString.push_back(char(97 + rand() % 25));
        }

        return univercityString;
    }

    void FillTableRandom(int count)
    {
        vector<university> universities = GetAllUniversities();
        auto begin = chrono::steady_clock::now();

        for (int i = 0; i < count; i++)
        {
            university a; 
            a.InitializeFromString(GenerateStringUniversity());
            universities.push_back(a);
            AddKey(a.code, a, false);
            
        }


        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << "\nВремя заполнения таблицы: " << time.count() << " нс\n";
        SaveDataToFile(universities);
    }
};


void WorkWithFile()
{
    cout << "Для работы с данными, будет автоматически считан файл data.bin\n";
    system("pause");
    vector<university> universities = LoadDataFromFile();
    cout << "Данные успешно считаны.\n";
    system("pause");

    while (true)
    {
        system("cls");
        cout << "Выберите действие:\n";
        cout << "1) Найти университет в файле по его номеру записи\n";
        cout << "2) Добавить университет в конец файла\n";
        cout << "3) Удалить университет из файла\n";
        cout << "4) Возврат назад\n";


        university result;
        string s;
        int num, ind;
        cin >> num;
        system("cls");
        auto begin = chrono::steady_clock::now();
        switch (num)
        {
        case 1:
            cout << "Введите порядковый номер записи в файле для поиска (индексация с 1): ";
            cin >> ind;

            result = ReadUniversityFromFile(ind);
            if (ind - 1 >= universities.size() || ind - 1 < 0)
                cout << "Записи с таким индексом не найдено\n";
            else
                cout << "Найденная запись:\nУниверситет " << result.name << "; Код университета: " << result.code << "\n";

            system("pause");
            break;
        case 2:
            cout << "Введите данные об университете в формате \"Код Название\": ";
            getline(cin, s);
            getline(cin, s);
            result.InitializeFromString(s);

            AddUniversirtyToFile(universities, result);
            cout << "Университет успешно добавлен в файл.\n";

            system("pause");
            //getline(cin, s);
            break;
        case 3:
            cout << "Введите порядковый номер записи в файле для удаления (индексация с 1): ";
            ind;
            cin >> ind;

            if (ind - 1 < universities.size() && ind - 1 >= 0)
            {
                result = ReadUniversityFromFile(ind);
                DeleteUniversirtyDromFile(universities, ind - 1);
                cout << "Город " << result.name << "(код " << result.code << ") Успешно удален.\n";
            }
            else cout << "Записи с таким индексом не существует. Удаление невозможно\n";

            system("pause");
            break;
        case 4:
            return;
            break;
        default:
            cout << "Неизвестная команда.\n";
            system("pause");
            break;
        }
        auto end = chrono::steady_clock::now();
        auto time = chrono::duration_cast<chrono::nanoseconds>(end - begin);
        cout << " Время работы: " << time.count() << " нс";
    }
}


void WorkWithHashTable()
{
    cout << "Для работы с данными, будет автоматически считан файл data.bin\n";
    system("pause");
    // vector<university> universities = LoadDataFromFile();
    HashTable ht = HashTable();
    cout << "Данные успешно считаны.\n";
    system("pause");

    while (true)
    {
        system("cls");
        cout << "Выберите действие:\n";
        cout << "1) Вывести всю хеш-таблицу\n";
        cout << "2) Вставить университет по его ключу (коду)\n";
        cout << "3) Удалить университет по его ключу (коду)\n";
        cout << "4) Найти университет по его ключу (коду)\n";
        cout << "5) Рехешировать таблицу (увеличение в 2 раза)\n";
        cout << "6) Заполнить таблицу случайными данными\n";
        cout << "7) Возврат назад\n";

        element* res;
        string s;
        int num, code, del;
        cin >> num;
        system("cls");
        switch (num)
        {
        case 1:
            ht.PrintTable(true);
            //system("pause");
            break;
        case 2:
            cout << "Введите данные об университете в формате \"Код Название\": ";
            getline(cin, s);
            getline(cin, s);
            university result;
            result.InitializeFromString(s);

            cout << "Университет успешно добавлен в таблицу. Хеш для него: " << ht.GetHash(result.code);
            if (ht.AddKey(result.code, result, true) == 1)
                cout << "\nТакой индекс не занят, поэтому в таблицу добавлена новая цепочка с одним элементом.\n";
            else
                cout << "\nЭтот индекс занят, поэтому элемент добавлен в начало соответствующей цепочки.\n";

            //system("pause");
            break;
        case 3:
            cout << "Введите код университета для удаления: ";
            cin >> code;
            del = ht.DeleteKey(code, true);

            if (del == -1)
                cout << "Университет с кодом " << code << " не найден в таблице. Удаление невозможно\n";
            if (del == 1)
                cout << "Университет с кодом " << code << " успешно удален. Он был единственным в своей цепочке\n";
            if (del == 2)
                cout << "Университет с кодом " << code << " успешно удален из своей цепочки\n";

            //system("pause");
            break;
        case 4:
            cout << "Введите код университета для поиска: ";
            code;
            cin >> code;
            res = ht.FindByKey(code, true);

            if (res == NULL)
                cout << "Университет с кодом " << code << " не найден в таблице.\n";
            else
                cout << "Найденный элемент:\nХеш " << ht.GetHash(code) << "; Университет " << res->value.name << "; Код университета : " << res->value.code << "\n";

            //system("pause");
            break;
        case 5:
            ht.Rehash(true);
            cout << "Таблица успешно рехеширована. Новая таблица:\n";
            ht.PrintTable(false);
            //system("pause");
            break;
        case 6:
            cout << "Введите количество генерируемых университетов: ";
            cin >> code;
            ht.FillTableRandom(code);
            cout << "Таблица успешно заполнена.\n";
            ht.PrintTable(false);
            break;
        case 7:
            return;
            break;
        default:
            cout << "Неизвестная команда.\n";
            //system("pause");
            break;
        }
        system("pause");
    }
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));


    while (true)
    {
        cout << "Выберите вид работы:\n";
        cout << "1) Работа с файлом\n";
        cout << "2) Работа с хеш таблицей\n";
        cout << "3) Выход\n";

        int num;
        cin >> num;
        system("cls");
        switch (num)
        {
        case 1:
            WorkWithFile();
            break;
        case 2:
            WorkWithHashTable();
            break;
        case 3:
            return 0;
            break;
        default:
            cout << "Неизвестная команда.";
            break;
        }
    }
}

#include <iostream>
#include <Windows.h>

using namespace std;

struct Node
{
    char value;
    Node* left;
    Node* right;
};

Node* MakeOperand(char num)
{
    Node* a = new Node();
    a->left = nullptr;
    a->right = nullptr;
    a->value = num;
    return a;
}

Node* MakeOperator(Node* left, char oper, Node* right)
{
    Node* a = new Node();
    a->left = left;
    a->right = right;
    a->value = oper;
    return a;
}



Node* MakeFromExpresstion(string s)
{
    int i = 0;
    Node* left = nullptr;
    while (i < s.length())
    {
        if (left == nullptr)
            left = MakeOperand(s[i]);

        if (i + 2 < s.length())
        {
            char oper = s[i + 1];
            int offset = i + 2;
            Node* right = MakeOperand(s[i + 2]);

            if (oper == '*' || oper == '/')
            {
                left = MakeOperator(left, oper, right);
                i += 2;
                continue;
            }

            while (offset + 2 < s.length())
            {
                char multi = s[offset + 1];
                if (multi != '*' && multi != '/')
                    break;

                Node* add = MakeOperand(s[offset + 2]);
                right = MakeOperator(right, multi, add);
                offset += 2;
            }
            i = offset;
            left = MakeOperator(left, oper, right);
        }
        else break;
    }
    return left;
}


string GetPrefixString(Node* node, string& s)
{
    if (node == nullptr)
        return s;

    s += node->value;
    GetPrefixString(node->left, s);
    GetPrefixString(node->right, s);

    return s;
}

Node* MakeFromPrefix(string s, int& i)
{
    char c = s[i];
    if (c == '+' || c == '-' || c == '*' || c == '/') // operator
    {
        Node* left = MakeFromPrefix(s, ++i);
        Node* right = MakeFromPrefix(s, ++i);
        return MakeOperator(left, c, right);
    }
    else // digit/letter
        return MakeOperand(c);
}


void PrintNode(Node* node, int n)
{
    if (node == nullptr)
        return;
    if (node->right == nullptr) // тогда и left == nullptrt => лист
    {
        for (int i = 0; i < n; i++)
            cout << '\t';
        cout << node->value << '\n';
        return;
    }

    PrintNode(node->right, n + 1);
    for (int i = 0; i < n; i++)
        cout << '\t';
    cout << node->value << '\n';
    PrintNode(node->left, n + 1);
}

void PrintTree(Node* tree)
{
    PrintNode(tree->right, 1);
    cout << tree->value << '\n';
    PrintNode(tree->left, 1);
}



int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (true)
    {
        system("cls");
        cout << "Выберите вариант работы программы:\n";
        cout << "1) Базовое задание (выражение с цифрами и знаками +, -, *)\n";
        cout << "2) Задание варианта (префиксное выражение с символами и знаками +, -, *, /)\n";
        cout << "3) Завершение работы\n";

        int num, i = 0;
        string s;
        Node* tree;
        cin >> num;
            system("cls");
        switch (num)
        {
        case 1:
            cout << "Введите выражение обычного вида с цифрами и операциями +, -, *\n";
            cin >> s;

            cout << "Дерево, построенное по выражению (повернутое на 90 градусов вправо):\n\n";
            tree = MakeFromExpresstion(s);
            PrintTree(tree);

            s = "";
            s = GetPrefixString(tree, s);
            cout << "Префиксное выражение для дерева: " << s << '\n';
            break;
        case 2:
            cout << "Введите префиксное выражение с символами и операциями +, -, *, /\n";
            cin >> s;
            i = 0;

            cout << "Дерево, построенное по префиксному выражению (повернутое на 90 градусов вправо):\n\n";
            tree = MakeFromPrefix(s, i);
            PrintTree(tree);
            break;
        case 3:
            return 0;
        default:
            cout << "Неизвестная команда!\n";
            break;
        }
        system("pause");
    }

    return 0;
}

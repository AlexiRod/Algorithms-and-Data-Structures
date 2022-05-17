#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include<windows.h>

using namespace std;

template<typename T>
struct Node
{
    T data;
    int index;

    Node(T d, int ind)
    {
        data = d;
        index = ind;
    }
};

template<typename T>
class Graph
{
    vector<vector<int>> matrix;
    vector<Node<T>> nodes;
    int size = 0;

public:

    void AddNode(T data)
    {
        Node<T> node(data, size);
        vector<int> row(++size, 0);

        for (int i = 0; i < matrix.size(); i++)
            matrix[i].push_back(0); // пути нет

        matrix.push_back(row);
        nodes.push_back(node);
    }

    bool AddPath(int a, int b, int val)
    {
        if (a < 0 || a >= size || b < 0 || b >= size)
            return false;

        matrix[a][b] = val;
        matrix[b][a] = val;

        return true;
    }


    void CheckConnected()
    {
        if (size == 0)
        {
            cout << "Данный граф пустой\n";
            return;
        }

        if (PrintBFS(0, false))
            cout << "Данный граф является связным\n";
        else
            cout << "Данный граф не является cвязным\n";
    }

    void PrintMatrix()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
                cout << matrix[i][j] << " ";
            cout << '\n';
        }
    }


    bool PrintBFS(int first, bool isPrint = true)
    {
        if (first < 0 || first >= size)
        {
            cout << "Неправильный индекс вершины\n";
            return false;
        }

        vector<bool> used(size, false);
        queue<Node<T>> q;
        Node<T> firstNode = nodes[first];
        q.push(firstNode);

        if (isPrint)
            cout << "Обход в ширину:\n";

        while (!q.empty())
        {
            Node<T> node = q.front();
            q.pop();
            used[node.index] = true;

            for (int i = 0; i < size; i++)
            {
                if (matrix[node.index][i] && !used[i])
                {
                    q.push(nodes[i]);
                    used[i] = true;
                }
            }
            if (isPrint)
                cout << node.data << " ";
        }
        if (isPrint)
            cout << '\n';

        for (int i = 0; i < size; i++)
            if (!used[i])
                return false;
        return true;
    }

    void PrintDFS(int first)
    {
        if (first < 0 || first >= size)
        {
            cout << "Неправильный индекс вершины\n";
            return;
        }

        vector<int> used(size, 0);
        stack<Node<T>> s;
        Node<T> firstNode = nodes[first];
        s.push(firstNode);

        cout << "Обход в глубину:\n";
        while (!s.empty())
        {
            Node<T> node = s.top();
            s.pop();

            if (used[node.index] == 2)
                continue;
            used[node.index] = 2;

            for (int i = 0; i < size; i++)
            {
                if (matrix[node.index][i] && used[i] != 2)
                {
                    s.push(nodes[i]);
                    used[i] = 1;
                }
            }
            cout << node.data << " ";
        }
        cout << '\n';
    }


    void FindPathWithDijkstra(int start, int end)
    {
        vector<Node<T>*> prevs(size, nullptr);
        vector<int> path(size, INT_MAX);
        vector<bool> used(size, false);
        path[start] = 0;

        for (int k = 1; k < size; k++)
        {
            int min = INT_MAX;
            int index = -1;
            for (int i = 0; i < size; i++)
                if (!used[i] && path[i] <= min)
                {
                    min = path[i];
                    index = i;
                }

            used[index] = true;

            for (int i = 0; i < size; i++)
                if (!used[i] && matrix[index][i] && path[index] != INT_MAX && path[index] + matrix[index][i] < path[i])
                {
                    path[i] = path[index] + matrix[index][i];
                    prevs[i] = &nodes[index];
                }


        }

        if (path[end] == INT_MAX)
        {
            cout << "Не удалось найти путь из вершины №" << start << " в вершину №" << end <<
                ".\nПроверьте, что граф связный между данными вершинами действительно есть путь.\n";
            return;
        }

        cout << "Длина пути из вершины №" << start << " в вершину №" << end << " равен " << path[end] << "\nПуть: ";

        vector<Node<T>*> route(1, &nodes[end]);
        int i = prevs[end]->index;
        while (true)
        {
            if (prevs[i] == nullptr)
                break;

            route.push_back(&nodes[i]);
            i = prevs[i]->index;
        }
        route.push_back(&nodes[start]);

        for (int i = route.size() - 1; i > 0; i--)
            cout << route[i]->data << " -> ";
        cout << route[0]->data << "\n";
    }
};


// Начальная инициализация графа по рис. №10
void FillGraphN10(Graph<int>& graph) {
    graph.AddNode(1);
    graph.AddNode(2);
    graph.AddNode(3);
    graph.AddNode(4);
    graph.AddNode(5);
    graph.AddNode(6);
    graph.AddNode(7);
    graph.AddNode(8);

    graph.AddPath(0, 1, 23);
    graph.AddPath(0, 2, 12);
    graph.AddPath(1, 2, 25);
    graph.AddPath(1, 4, 22);
    graph.AddPath(1, 7, 35);
    graph.AddPath(2, 3, 18);
    graph.AddPath(3, 5, 20);
    graph.AddPath(4, 5, 23);
    graph.AddPath(4, 6, 14);
    graph.AddPath(5, 6, 24);
    graph.AddPath(6, 7, 16);
}

void WorkWithGraph()
{
    Graph<int> graph;
    FillGraphN10(graph);

    while (true)
    {
        system("cls");
        cout << "Выберите действие:\n";
        cout << "1) Добавить вершину\n";
        cout << "2) Добавить путь\n";
        cout << "3) Вывести матрицу смежности графа\n";
        cout << "4) Вывести обход графа в ширину (BFS)\n";
        cout << "5) Вывести обход графа в глубину (DFS)\n";
        cout << "6) Проверить граф на связность\n";
        cout << "7) Найти путь методом Дейкстры\n";
        cout << "8) Выход\n";

        int num, val, a, b;
        cin >> num;
        system("cls");

        switch (num)
        {
        case 1:
            cout << "Введите значение новой вершины (целое число): ";
            cin >> val;
            graph.AddNode(val);
            cout << "Вершина успешно добавлена.\n";
            break;
        case 2:
            cout << "Введите индексы двух вершин (начиная с 0), между которыми надо проложить путь, и его стоимость через пробел: ";
            cin >> a >> b >> val;
            if (graph.AddPath(a, b, val))
                cout << "Путь успешно построен\n";
            else
                cout << "Индексы введены неправильно\n";
            break;
        case 3:
            cout << "Матрица смежности графа:\n";
            graph.PrintMatrix();
            break;
        case 4:
            cout << "Введите индекс вершины (начиная с 0), с которой надо начать обход: ";
            cin >> val;
            graph.PrintBFS(val);
            break;
        case 5:
            cout << "Введите индекс вершины (начиная с 0), с которой надо начать обход: ";
            cin >> val;
            graph.PrintDFS(val);
            break;
        case 6:
            graph.CheckConnected();
            break;
        case 7:
            cout << "Введите индексы двух вершин (начиная с 0), между которыми надо найти оптимальный путь: ";
            cin >> a >> b;
            graph.FindPathWithDijkstra(a, b);
            break;
        case 8:
            return;
        default:
            cout << "Неизвестная команда.\n";
            break;
        }
        system("pause");
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WorkWithGraph();
}

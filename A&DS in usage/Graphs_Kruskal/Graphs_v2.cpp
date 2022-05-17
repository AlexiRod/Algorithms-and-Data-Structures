#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

struct Edge {
    int from, to, weight, id;
    bool is_bridge;
    Edge* neighbour;

    Edge(int from_, int to_, int weight_, int id_)
    {
        from = from_;
        to = to_;
        weight = weight_;
        id = id_;
        is_bridge = false;
        neighbour = nullptr;
    }
};

class Graph
{
public:

    Graph(int n_)
    {
        n = n_;
        graph.resize(n);
        childs.resize(n);
        deeps.resize(n);
        used.resize(n);

        parents_ = new int[n];
        sizes_ = new int[n];
        for (int i = 0; i < n; ++i) {
            parents_[i] = i;
            sizes_[i] = 1;
        }
    }


    void AddEdge(int from, int to, int weight)
    {
        Edge* a = new Edge(from, to, weight, id);
        Edge* b = new Edge(to, from, weight, id++);
        a->neighbour = b;
        b->neighbour = a;

        edges.push_back(a);
        graph[from].push_back(a);
        graph[to].push_back(b);
    }


    void FindBridges()
    {
        for (int i = 0; i < n; i++)
        {
            childs[i] = 0;
            deeps[i] = 0;
            used[i] = false;
        }
        bridges.clear();
        sort(edges.begin(), edges.end(), IDComparator);

        for (int i = 0; i < n; i++)
            if (!used[i])
                DFS(i);

        sort(bridges.begin(), bridges.end());
        cout << "Вершины, являющиеся мостами:\n";
        for (auto ind : bridges)
            cout << edges[ind-1]->from + 1 << " <-> " << edges[ind-1]->to + 1 << " (" << edges[ind-1]->weight << ")\n";
    }


    void KruskalAlgorighm() {
        vector<Edge*> res;
        sort(edges.begin(), edges.end(), WeightComparator);
        for (int i = 0; i < n; ++i) {
            parents_[i] = i;
            sizes_[i] = 1;
        }

        for (int i = 0; i < id; ++i) {
            int from = edges[i]->from;
            int to = edges[i]->to;

            if (GetDSU(from) != GetDSU(to)) {
                res.push_back(edges[i]);
                UniteDSU(from, to);
            }
        }
        Print(false, res);
    }


    void Print(bool isEdges = true, vector<Edge*> vec = vector<Edge*>())
    {
        if (isEdges)
        {
            cout << "Список смежных вершин графа:\n";
            for (int i = 0; i < edges.size(); i++)
                cout << edges[i]->from + 1 << " <-> " << edges[i]->to + 1 << " = " << edges[i]->weight << " (" << edges[i]->id << ")\n";
        }
        else
        {
            sort(vec.begin(), vec.end(), FromComparator);
            cout << "Минимальное остовное дерево:\n";
            for (int i = 0; i < vec.size(); i++)
                cout << vec[i]->from + 1 << " <-> " << vec[i]->to + 1 << " = " << vec[i]->weight << " (" << vec[i]->id << ")\n";
        }
    }

private:

    int id = 0, n;
    vector<Edge*> edges;
    vector<vector<Edge*>> graph;
    vector<int> childs, deeps, bridges;
    vector<bool> used;

    int* parents_;
    int* sizes_;

    void DFS(int to, int depth = 0, Edge* neigh = nullptr)
    {
        used[to] = true;
        childs[to] = deeps[to] = depth;
        for (Edge* edge : graph[to])
        {
            if (!used[edge->to])
            {
                DFS(edge->to, depth + 1, edge);
                childs[to] = min(childs[to], childs[edge->to]);
            }
            else if (neigh && neigh->id != edge->id)
                childs[to] = min(childs[to], deeps[edge->to]);
        }

        if (neigh != nullptr && childs[to] == deeps[to])
        {
            neigh->is_bridge = neigh->neighbour->is_bridge = true;
            bridges.push_back(neigh->id + 1);
        }
    }

    int GetDSU(int index) {
        if (index == parents_[index]) {
            return index;
        }
        return parents_[index] = GetDSU(parents_[index]);
    }

    void UniteDSU(int first_ind, int second_ind) {
        first_ind = GetDSU(first_ind);
        second_ind = GetDSU(second_ind);

        if (first_ind == second_ind) {
            return;
        }

        if (sizes_[first_ind] < sizes_[second_ind]) {
            parents_[first_ind] = second_ind;
            sizes_[second_ind] += sizes_[first_ind];
        }
        else {
            parents_[second_ind] = first_ind;
            sizes_[first_ind] += sizes_[second_ind];
        }
    }

    static int WeightComparator(const void* fitst, const void* second)
    {
        Edge a = *(Edge*)fitst;
        Edge b = *(Edge*)second;
        return a.weight < b.weight;
    }

    static int FromComparator(const void* fitst, const void* second)
    {
        Edge a = *(Edge*)fitst;
        Edge b = *(Edge*)second;
        return a.from < b.from;
    }

    static int IDComparator(const void* fitst, const void* second)
    {
        Edge a = *(Edge*)fitst;
        Edge b = *(Edge*)second;
        return a.id < b.id;
    }
};


void Fill(Graph &graph)
{
    graph.AddEdge(0, 1, 4);
    graph.AddEdge(0, 2, 1);
    graph.AddEdge(1, 2, 5);
    graph.AddEdge(2, 3, 6);
    graph.AddEdge(3, 4, 10);
    graph.AddEdge(4, 7, 12);
    graph.AddEdge(4, 5, 4);
    graph.AddEdge(5, 6, 4);
    graph.AddEdge(5, 8, 3);
    graph.AddEdge(6, 8, 2);
}

int main()
{
    setlocale(LC_ALL, "");
    int  n, num, from, to, val;
    cout << "Введите количество вершин в графе: ";
    cin >> n;
    Graph graph(n);
    Fill(graph);

    while (true)
    {
        system("cls");
        cout << "Выберите команду:\n";
        cout << "1. Добавить ребро\n";
        cout << "2. Вывести список смежных вершин\n";
        cout << "3. Найти мосты в графе\n";
        cout << "4. Найти минимальный каркас\n";
        cout << "5. Выйти\n";

        cin >> num;
        system("cls");

        switch (num)
        {
        case 1:
            cout << "Введите через пробел номера соединяемых вершин, а затем вес ребра: ";
            cin >> from >> to >> val;
            graph.AddEdge(from - 1, to - 1, val);
            cout << "Ребро успешно добавлено.\n";
            break;
        case 2:
            graph.Print();
            break;
        case 3:
            graph.FindBridges();
            break;
        case 4:
            graph.KruskalAlgorighm();
            break;
        case 5:
            return 0;
        default:
            cout << "Неправильная команда.\n";
            break;
        }
        system("pause");
    }
}

#include <iostream>

struct Edge {
    int from;
    int to;
    int weight;

    Edge() {
        from = to = weight = 0;
    }

    Edge(int from, int to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};

class Solver {
    Edge* edges_;
    int* parents_;
    int* sizes_;
    int vertex_count_, edges_count_;

public:
    Solver(int vertex_count, int edges_count) {
        this->vertex_count_ = vertex_count;
        this->edges_count_ = edges_count;

        parents_ = new int[vertex_count];
        sizes_ = new int[vertex_count];
        edges_ = new Edge[edges_count];

        for (int i = 0; i < vertex_count; ++i) {
            parents_[i] = i;
            sizes_[i] = 1;
        }
    }

    ~Solver() {
        delete[] edges_;
        delete[] parents_;
        delete[] sizes_;
    }

    int getDSU(int index) {
        if (index == parents_[index]) {
            return index;
        }
        return parents_[index] = getDSU(parents_[index]);
    }

    void uniteDSU(int first_ind, int second_ind) {
        first_ind = getDSU(first_ind);
        second_ind = getDSU(second_ind);

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

    uint64_t solveKruskal() {
        uint64_t sum = 0;
        qickSort(edges_, edges_count_);

        for (int i = 0; i < edges_count_; ++i) {
            int from = edges_[i].from;
            int to = edges_[i].to;

            if (getDSU(from) != getDSU(to)) {
                sum += edges_[i].weight;
                uniteDSU(from, to);
            }
        }
        return sum;
    }

    void readGraph() {
        int from, to, weight;
        for (int i = 0; i < edges_count_; ++i) {
            std::cin >> from >> to >> weight;
            edges_[i] = Edge(from - 1, to - 1, weight);
        }
    }

    void qickSort(Edge* mas, int size) {
        int left = 0;
        int right = size - 1;
        Edge mid = mas[size / 2];

        do {
            while (mas[left].weight < mid.weight) {
                left++;
            }

            while (mas[right].weight > mid.weight) {
                right--;
            }

            if (left <= right) {
                Edge tmp = mas[left];
                mas[left] = mas[right];
                mas[right] = tmp;

                left++;
                right--;
            }
        } while (left <= right);

        if (right > 0) {
            qickSort(mas, right + 1);
        }
        if (left < size) {
            qickSort(&mas[left], size - left);
        }
    }
};

int main() {
    int vertexes_count, edges_count;
    std::cin >> vertexes_count >> edges_count;

    Solver solver(vertexes_count, edges_count);
    solver.readGraph();
    std::cout << solver.solveKruskal();
}

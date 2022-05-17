#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

void findItems(vector<int>* a, const vector<int>& p, const vector<vector<int>>& d, int i, int j) {
    if (d[i][j] == 0) {
        return;
    }
    if (d[i - 1][j] == d[i][j]) {
        findItems(a, p, d, i - 1, j);
    } else {
        findItems(a, p, d, i - 1, j - p[i - 1]);
        a->push_back(i);
    }
}

int main() {
    int n, w;
    std::cin >> n >> w;
    vector<int> p(n), c(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> p[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> c[i];
    }
    vector<vector<int>> d;
    for (int i = 0; i < n + 1; ++i) {
        d.push_back(vector<int>(w + 1));
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= w; ++j) {
            if (j >= p[i - 1]) {
                d[i][j] = std::max(d[i - 1][j], d[i - 1][j - p[i - 1]] + c[i - 1]);
            } else {
                d[i][j] = d[i - 1][j];
            }
        }
    }
    vector<int> a;
    findItems(&a, p, d, n, w);

    int tot_value = 0, tot_weight = 0;
    for (int i = 0; i < a.size(); ++i) {
        tot_value += c[a[i] - 1];
        tot_weight += p[a[i] - 1];
    }
    std::cout << tot_value << "\n" << tot_weight << "\n" << a.size() << "\n";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << p[a[i] - 1] << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << c[a[i] - 1] << " ";
    }
}

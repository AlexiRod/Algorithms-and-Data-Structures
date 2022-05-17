#include <iostream>
#include <algorithm>
#include <vector>

struct Item {
    int value, weight;
    Item(int value, int weight) : value(value), weight(weight) {
    }
};

bool cmp(struct Item a, struct Item b) {
    double r1 = static_cast<double>(a.value) / a.weight;
    double r2 = static_cast<double>(b.value) / b.weight;
    if (r1 == r2) {
        return a.value > b.value;
    }
    return r1 > r2;
}

int main() {
    int n, w, c;
    std::cin >> n >> w;
    std::vector<int> p(n);
    std::vector<Item> arr, ans;
    for (int i = 0; i < n; ++i) {
        std::cin >> p[i];
    }
    for (int i = 0; i < n; ++i) {
        std::cin >> c;
        arr.push_back({ c, p[i] });
    }

    std::sort(arr.begin(), arr.end(), cmp);
    int cur_weight = 0, tot_value = 0;
    for (int i = 0; i < n; ++i) {
        if (cur_weight + arr[i].weight <= w) {
            cur_weight += arr[i].weight;
            tot_value += arr[i].value;
            ans.push_back(arr[i]);
        }
    }
    std::cout << tot_value << "\n" << cur_weight << "\n" << ans.size() << "\n";
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i].weight << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i].value << " ";
    }
}

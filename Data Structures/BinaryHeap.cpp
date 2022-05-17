#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> left, right;
int inf = INT32_MAX, rmin = INT32_MAX;

void pushBack(int elem) {
    right.push_back(elem);
    rmin = std::min(rmin, elem);
}

void popFront() {
    if (!left.empty()) {
        left.pop_back();
        return;
    }

    rmin = inf;
    int minimum = inf;
    while (!right.empty()) {
        minimum = std::min(minimum, right.back());
        left.push_back(minimum);
        right.pop_back();
    }
    left.pop_back();
}

int getMin() {
    return (!left.empty() ? std::min(rmin, left.back()) : rmin);
}

int main() {
    int nn, kk, elem;
    std::cin >> nn >> kk;
    for (int i = 0; i < nn; ++i) {
        std::cin >> elem;
        pushBack(elem);
        if (i >= kk - 1) {
            std::cout << getMin() << "\n";
            popFront();
        }
    }
}

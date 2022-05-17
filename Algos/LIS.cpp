#include <iostream>
#include <set>

int main() {
    int count, elem;
    std::cin >> count;
    std::set<int> a;

    for (int i = 0; i < count; ++i) {
        std::cin >> elem;
        a.insert(elem);
    }

    int max_len = 1, len = 1, prev = *(a.begin());
    std::set<int>::iterator number = ++(a.begin());
    for (; number != a.end(); ++number) {
        if (*number != prev + 1) {
            max_len = max_len > len ? max_len : len;
            len = 0;
        }
        prev = *number;
        ++len;
    }
    std::cout << (max_len > len ? max_len : len);
    return 0;
}

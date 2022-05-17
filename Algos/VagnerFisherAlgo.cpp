#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

int dlAlgo(std::string fir, std::string sec) {
    if (fir.size() > sec.size()) {
        std::swap(fir, sec);
    }

    int min = fir.size(), max = sec.size();
    std::vector<int> d(min + 1);
    for (int i = 0; i < min + 1; ++i) {
        d[i] = i;
    }

    for (int ind = 1; ind < max + 1; ++ind) {
        int p = d[0]++, pp = 0;
        for (int j = 1; j < min + 1; ++j) {
            pp = d[j];
            if (fir[j - 1] == sec[ind - 1]) {
                d[j] = p;
            } else {
                d[j] = std::min(std::min(d[j], d[j - 1]), p) + 1;
            }
            p = pp;
        }
    }
    return d[min];
}

int main() {
    std::string fir, sec;
    std::getline(std::cin, fir);
    std::getline(std::cin, sec);
    std::cout << dlAlgo(fir, sec);
    return 0;
}

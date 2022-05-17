#include <iostream>
#include <string>
#include <vector>


void prefFunc(std::vector<int>* arr, const std::string& s) {
    for (int i = 1; i < s.size(); ++i) {
        int j = arr->at(i - 1);
        while (0 < j && s[i] != s[j]) {
            j = arr->at(j - 1);
        }
        if (s[i] == s[j]) {
            arr->at(i) = j + 1;
        }
    }
}

std::vector<int> kmpAlgo(const std::string& s, const std::string& t) {
    std::vector<int> a(s.size(), 0), res;
    prefFunc(&a, s);

    int i = 0, j = 0;
    while (i < t.size()) {
        if (t[i] == s[j]) {
            i++;
            j++;
        }
        if (j == s.size()) {
            j = a[j - 1];
            res.push_back(i - s.size());
        } else if (i < t.size() && t[i] != s[j]) {
            if (j != 0) {
                j = a[j - 1];
            }
            else {
                i++;
            }
        }
    }
    return res;
}

int main() {
    std::string s, t;
    std::getline(std::cin, s);
    std::getline(std::cin, t);
    std::vector<int> res = kmpAlgo(s, t);
    std::cout << res.size() << "\n";
    for (int& elem : res) {
        std::cout << elem << "\n";
    }
    return 0;
}

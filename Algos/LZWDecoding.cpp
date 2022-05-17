#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;

void decode(vector<int> codes) {
    std::unordered_map<int, string> table;
    int code = 0;
    for (; code <= 127; ++code) {
        string symb;
        symb += static_cast<char>(code);
        table[code] = symb;
    }

    int x = codes[0], y;
    string str_x = table[x], str_y;
    str_y += str_x[0];
    std::cout << str_x;
    for (int ind = 0; ind < codes.size() - 1; ++ind) {
        y = codes[ind + 1];

        if (table.find(y) != table.end()) {
            str_x = table[y];
        } else {
            str_x = table[x];
            str_x = str_x + str_y;
        }

        std::cout << str_x;
        str_y = "";
        str_y += str_x[0];
        table[code++] = table[x] + str_y;
        x = y;
    }
}

int main() {
    int n;
    std::cin >> n;
    vector<int> codes(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> codes[i];
    }
    decode(codes);
}

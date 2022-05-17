#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;

vector<int> encode(string str) {
    std::unordered_map<string, int> table;
    int code = 0;
    for (; code <= 127; ++code) {
        string ch;
        ch += static_cast<char>(code);
        table[ch] = code;
    }

    vector<int> codes;
    string x, y;
    x += str[0];
    for (int ind = 0; ind < str.length(); ++ind) {
        if (ind < str.length() - 1) {
            y += str[ind + 1];
        }
        string xy = x + y;
        if (table.find(xy) != table.end()) {
            x = xy;
        } else {
            codes.push_back(table[x]);
            table[xy] = code++;
            x = y;
        }
        y = "";
    }
    codes.push_back(table[x]);
    return codes;
}

int main() {
    string s;
    std::getline(std::cin, s);
    vector<int> codes = encode(s);
    std::cout << codes.size() << "\n";
    for (int i = 0; i < codes.size(); ++i) {
        std::cout << codes[i] << " ";
    }
}

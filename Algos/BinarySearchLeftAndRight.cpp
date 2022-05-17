#include <iostream>
#include <map>
#include <string>

int main() {
    int nn, mm, elem;
    std::cin >> nn >> mm;
    int* arr = new int[nn];

    for (int i = 0; i < nn; ++i) {
        std::cin >> elem;
        arr[i] = elem;
    }

    std::map<int, std::string> founds;
    for (int ind = 0; ind < mm; ++ind) {
        std::cin >> elem;
        int left = -1;
        int right = nn;
        bool was_found = false;
        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            if (arr[mid] == elem) {
                auto found_it = founds.find(elem);
                if (found_it != founds.end()) {
                    std::cout << (*found_it).second << "\n";
                    was_found = true;
                    break;
                }

                int ll = mid, rr = mid;
                while (ll > 0 && arr[ll - 1] == elem) {
                    ll--;
                }
                while (rr < nn - 1 && arr[rr + 1] == elem) {
                    rr++;
                }
                std::cout << ++ll << " " << ++rr << "\n";
                founds.insert({elem, (std::to_string(ll) + " " + std::to_string(rr))});
                was_found = true;
                break;
            }
            if (arr[mid] > elem) {
                right = mid;
            } else {
                left = mid;
            }
        }
        if (!was_found) {
            std::cout << "0\n";
            founds.insert({elem, "0"});
        }
    }
    delete[] arr;
}

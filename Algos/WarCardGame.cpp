#include <iostream>
#include <deque>

int makeTurn(std::deque<int>* first, std::deque<int>* second, int count) {
    int top_first = first->front();
    int top_second = second->front();
    if (top_first > top_second && !(top_second == 0 && top_first == 9) ||
        top_first == 0 && top_second == 9) {
        first->push_back(top_first);
        first->push_back(top_second);
    } else {
        second->push_back(top_first);
        second->push_back(top_second);
    }

    first->pop_front();
    second->pop_front();

    if (first->empty()) {
        return -1;
    }
    if (second->empty()) {
        return -2;
    }
    return ++count;
}

int main() {
    std::deque<int> first;
    std::deque<int> second;

    int card;
    for (int i = 0; i < 5; ++i) {
        std::cin >> card;
        first.push_back(card);
    }

    for (int i = 0; i < 5; ++i) {
        std::cin >> card;
        second.push_back(card);
    }

    int count = 0, result = 0;
    do {
        result = makeTurn(&first, &second, result);
        if (result == -1) {
            std::cout << "second " << count + 1;
            return 0;
        }
        if (result == -2) {
            std::cout << "first " << count + 1;
            return 0;
        }
        count = result;
    } while (count < 1000000);
    std::cout << "botva";
    return 0;
}

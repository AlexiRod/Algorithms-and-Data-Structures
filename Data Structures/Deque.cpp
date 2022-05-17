#include <iostream>
#include <string>

using std::string;
const int MAX_SIZE = 100;

template<typename T>
class Deque {
 private:
    T* arr;
    int _front, _back, _size, count;

 public:
    Deque() {
        arr = new T[MAX_SIZE];
        _size = MAX_SIZE;
        _front = -1;
        _back = 0;
        count = 0;
    }
    ~Deque() {
        delete[] arr;
    }

    string push_front(T n) {
        if (_front == -1) {
            _front = _back = 0;
        } else if (_front == 0) {
            _front = _size - 1;
        } else {
            --_front;
        }

        ++count;
        arr[_front] = n;
        return "ok";
    }

    string push_back(T n) {
        if (_front == -1) {
            _front = _back = 0;
        } else if (_back == _size - 1) {
            _back = 0;
        } else {
            ++_back;
        }

        ++count;
        arr[_back] = n;
        return "ok";
    }

    string pop_front() {
        if (_front == -1) {
            return "error";
        }
        string ret = std::to_string(arr[_front]);
        if (_front == _back) {
            _front = _back = -1;
        } else if (_front == _size - 1) {
            _front = 0;
        } else {
            ++_front;
        }

        --count;
        return ret;
    }

    string pop_back() {
        if (_front == -1) {
            return "error";
        }
        string ret = std::to_string(arr[_back]);
        if (_front == _back) {
            _front = _back = -1;
        } else if (_back == 0) {
            _back = _size - 1;;
        } else {
            --_back;
        }

        --count;
        return ret;
    }

    string front() {
        if (_front == -1) {
            return "error";
        }
        return std::to_string(arr[_front]);
    }

    string back() {
        if (_front == -1) {
            return "error";
        }
        return std::to_string(arr[_back]);
    }

    int size() {
        return count;
    }

    string clear() {
        _front = -1;
        _back = 0;
        count = 0;
        _size = MAX_SIZE;
        // Удаление элементов как таковое не требуется - дек будет перезаполнен.
        return "ok";
    }

    string exit() {
        return "bye";
    }
};


int main() {
    string s;
    int n;
    Deque<int> dq;

    do {
        std::cin >> s;
        if (s == "push_front") {
            std::cin >> n;
            std::cout << dq.push_front(n) << '\n';
        }
        if (s == "push_back") {
            std::cin >> n;
            std::cout << dq.push_back(n) << '\n';
        }
        if (s == "pop_front") {
            std::cout << dq.pop_front() << '\n';
        }
        if (s == "pop_back") {
            std::cout << dq.pop_back() << '\n';
        }
        if (s == "front") {
            std::cout << dq.front() << '\n';
        }
        if (s == "back") {
            std::cout << dq.back() << '\n';
        }
        if (s == "size") {
            std::cout << dq.size() << '\n';
        }
        if (s == "clear") {
            std::cout << dq.clear() << '\n';
        }
        if (s == "exit") {
            std::cout << dq.exit() << '\n';
        }
    } while (s != "exit");

    return 0;
}

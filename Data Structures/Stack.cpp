#include <iostream>
#include <string>

using std::string;

template<typename T>
class Stack {
 private:
    T* arr;
    int length, index;

    void Resize(int new_length) {
        T* new_arr = new T[new_length];
        for (int i = 0; i < index; ++i) {
            new_arr[i] = arr[i];
        }

        delete[] arr;
        length = new_length;
        arr = new_arr;
    }

 public:
    Stack() {
        index = 0;
        length = 1;
        arr = new T[length];
    }

    ~Stack() {
        delete[] arr;
    }

    string push(T n) {
        if (index + 1 >= length) {
            Resize(length * 2);
        }
        arr[index++] = n;
        return "ok";
    }

    string pop() {
        if (index == 0) {
            return "error";
        }

        return  std::to_string(arr[--index]);
    }

    string back() {
        if (index == 0) {
            return "error";
        }

        return std::to_string(arr[index-1]);
    }

    int size() {
        return index;
    }

    string clear() {
        index = 0;
        length = 1;
        T* new_arr = new T[1];
        delete[] arr;
        arr = new_arr;

        return "ok";
    }

    string exit() {
        return "bye";
    }
};


int main() {
    string s;
    Stack<int> st;
    do {
        std::cin >> s;
        if (s == "push") {
            int n;
            std::cin >> n;
            std::cout << st.push(n) << '\n';
        }
        if (s == "pop") {
            std::cout << st.pop() << '\n';
        }
        if (s == "back") {
            std::cout << st.back() << '\n';
        }
        if (s == "size") {
            std::cout << st.size() << '\n';
        }
        if (s == "clear") {
            std::cout << st.clear() << '\n';
        }
        if (s == "exit") {
            std::cout << st.exit() << '\n';
        }
    } while (s != "exit");

    return 0;
}

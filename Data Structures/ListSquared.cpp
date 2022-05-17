#include <iostream>
#include <string>

constexpr int kMaxSize = 2000000;

template <typename T>
class MyList {
private:
    int size_, mod_;
    char oper_;
    T* a_;

public:
    MyList(char oper, int mod, int size) {
        a_ = new T[kMaxSize];
        oper_ = oper;
        mod_ = mod;
        size_ = size;

        for (int i = 0; i < size; ++i) {
            std::cin >> a_[i];
        }
    }

    void insert(int ind, T data) {
        for (int i = size_; i > ind; --i) {
            a_[i] = a_[i - 1];
        }

        a_[ind] = data;
        ++size_;
    }

    void remove(int ind) {
        for (int i = ind; i < size_ - 1; ++i) {
            a_[i] = a_[i + 1];
        }

        --size_;
    }
    void set(int ind, T data) {
        a_[ind] = data;
    }

    int get(int ind) {
        return a_[ind];
    }

    int size() {
        return size_;
    }

    int64_t compute(int left, int right) {
        int64_t result = 0;

        if (oper_ == '+') {
            for (int i = left; i <= right; ++i) {
                result += a_[i];
            }
            result %= mod_;
            return result < 0 ? result + mod_ : result;
        } else {
            for (int i = left; i <= right; ++i) {
                result = result ^ a_[i];
            }
            return result;
        }
    }

    void printList() {
        for (int i = 0; i < size_; ++i) {
            std::cout << a_[i] << " ";
        }
        std::cout << "\n";
    }

    ~MyList() {
        delete[] a_;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int mod = -1, size, n, ind, data;
    std::string s;
    char oper;
    std::cin >> oper;
    if (oper == '+') {
        std::cin >> mod;
    }

    std::cin >> size;
    MyList<int> list = MyList<int>(oper, mod, size);
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> s;

        if (s == "insert") {
            std::cin >> ind >> data;
            list.insert(--ind, data);
        } else if (s == "erase") {
            std::cin >> ind;
            list.remove(--ind);
        } else if (s == "change") {
            std::cin >> ind >> data;
            list.set(--ind, data);
        } else if (s == "at") {
            std::cin >> ind;
            std::cout << list.get(--ind) << "\n";
        } else if (s == "compute") {
            std::cin >> ind >> data;
            std::cout << list.compute(--ind, --data) << "\n";
        } else if (s == "size") {
            std::cout << list.size() << "\n";
        }
    }
    return 0;
}

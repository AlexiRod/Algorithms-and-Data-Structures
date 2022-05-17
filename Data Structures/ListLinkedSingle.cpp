#include <iostream>
#include <string>

template <typename T>
struct Node {
    T data;
    Node<T>* next;

public:
    explicit Node(T d) {
        data = d;
        next = nullptr;
    }
};

template <typename T>
class MyList {
private:
    Node<T>* head_;
    Node<T>* tail_;
    int size_;

    void deleteNode(Node<T>* cur) {
        if (cur == nullptr) {
            return;
        }
        deleteNode(cur->next);
        delete cur;
        cur = nullptr;
    }

public:
    MyList() {
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr;
    }

    void pushBack(T data) {
        Node<T>* node = new Node<T>(data);
        ++size_;

        if (head_ == nullptr) {
            head_ = node;
            tail_ = node;
            return;
        }

        tail_->next = node;
        tail_ = node;
    }

    void insert(T data, int position) {
        if (position == size_ - 1) {
            pushBack(data);
            return;
        }

        Node<T>* node = new Node<T>(data);
        Node<T>* cur = head_;

        while (position-- != 0) {
            cur = cur->next;
        }

        node->next = cur->next;
        cur->next = node;
        ++size_;
    }

    void remove(int position) {
        bool is_tail = position == size_ - 2;

        Node<T>* cur = head_;
        while (position-- != 0) {
            cur = cur->next;
        }

        Node<T>* cur_ptr = cur->next;
        cur->next = cur->next->next;
        if (is_tail) {
            tail_ = cur;
        }

        delete cur_ptr;
        cur_ptr = nullptr;
        --size_;
    }

    void printList() {
        Node<T>* cur = head_;
        while (cur != nullptr) {
            std::cout << cur->data << " ";
            cur = cur->next;
        }
    }

    ~MyList() {
        deleteNode(head_);
        head_ = nullptr;
        tail_ = nullptr;
    }
};

int main() {
    int n, m, elem, pos;
    std::string s;
    std::cin >> n >> m;

    MyList<int>* a = new MyList<int>();

    for (int i = 0; i < n; ++i) {
        std::cin >> elem;
        a->pushBack(elem);
    }

    for (int i = 0; i < m; ++i) {
        std::cin >> s;
        if (s == "insert") {
            std::cin >> elem >> pos;
            a->insert(elem, pos);
        } else {
            std::cin >> pos;
            a->remove(pos);
        }
    }

    a->printList();
    delete a;

    return 0;
}

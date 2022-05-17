#include <iostream>
#include <string>

template <typename T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;

public:
    explicit Node(T d) {
        data = d;
        next = nullptr;
        prev = nullptr;
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

        node->prev = tail_;
        tail_->next = node;
        tail_ = node;
    }

    void pushFront(T data) {
        Node<T>* node = new Node<T>(data);
        ++size_;

        if (head_ == nullptr) {
            head_ = node;
            tail_ = node;
            return;
        }

        head_->prev = node;
        node->next = head_;
        head_ = node;
    }

    void insert(T data, int position) {
        Node<T>* node = new Node<T>(data);
        Node<T>* cur = head_;

        while (position-- != 0) {
            cur = cur->next;
        }

        node->next = cur->next;
        node->prev = cur;
        cur->next->prev = node;
        cur->next = node;
        ++size_;
    }

    void remove(int position) {
        if (position == size_ - 2) {
            Node<T>* tail_ptr = tail_;
            tail_->prev->next = nullptr;
            tail_ = tail_->prev;

            delete tail_ptr;
            tail_ptr = nullptr;
            size_--;
            return;
        }

        Node<T>* cur = head_;
        while (position-- != -1) {
            cur = cur->next;
        }

        Node<T>* cur_ptr = cur;
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;

        delete cur_ptr;
        cur_ptr = nullptr;
        --size_;
    }

    void printFront() {
        Node<T>* cur = head_;
        while (cur != nullptr) {
            std::cout << cur->data << " ";
            cur = cur->next;
        }
    }

    void printBack() {
        Node<T>* cur = tail_;
        while (cur != nullptr) {
            std::cout << cur->data << " ";
            cur = cur->prev;
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
        if (s == "push_front") {
            std::cin >> elem;
            a->pushFront(elem);
        }
        if (s == "push_back") {
            std::cin >> elem;
            a->pushBack(elem);
        }
        if (s == "insert") {
            std::cin >> elem >> pos;
            a->insert(elem, pos);
        }
        if (s == "remove") {
            std::cin >> pos;
            a->remove(pos);
        }
    }

    a->printFront();
    std::cout << '\n';
    a->printBack();
    delete a;

    return 0;
}

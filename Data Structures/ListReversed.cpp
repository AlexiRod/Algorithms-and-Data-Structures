#include <iostream>

// Represents basic element of list.
template <typename T>
class Node {
public:
    T value;
    // Link to the next element.
    Node* next;

    explicit Node(T value, Node* next = nullptr) {
        this->value = value;
        this->next = next;
    }
};

template <typename T>
class List {
private:
    Node<T>* head_;
    Node<T>* tail_;
    int size_;

public:
    List() {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }
    // Removes node and all its linked next elements recursively.
    void removeNode(Node<T>* node) {
        if (node == nullptr) {
            return;
        }
        removeNode(node->next);
        delete node;
        --size_;
         node = nullptr;
    }

    // Deletes all nodes of the list to prevents memory leaks.
    ~List() {
        if (head_ == nullptr) {
            return;
        }
        removeNode(head_);
        size_ = 0;
        head_ = nullptr;
        // check
        tail_ = head_;
    }

    // adds a new node with given value to the back of the list.
    void pushBackValue(T value) {
        auto inserted_node = new Node<T>(value);
        // Checking if the list is empty.
        if (head_ == nullptr) {
            head_ = inserted_node;
            tail_ = inserted_node;
            ++size_;
            return;
        }
        tail_->next = inserted_node;
        tail_ = inserted_node;
        ++size_;
    }

    // Deletes head of the list and returns the value.
    Node<T>* popFront() {
        Node<T> *current_head = head_;
        if (head_ == nullptr) {
            std::cout << "Error: List is empty";
            return current_head;
        }
        // Checking if there is only one element is left.
        if (size_ == 1) {
            head_ = nullptr;
            tail_ = nullptr;
            --size_;
            return current_head;
        }
        head_ = head_->next;
        --size_;
        return current_head;
    }

    int size() {
        return size_;
    }

    // Adding given amount of elements to the list.
    void fillList(int length) {
        int input;
        for (int i = 0; i < length; ++i) {
            std::cin >> input;
            pushBackValue(input);
        }
        size_ = length;
    }

    // Adds new Node with given value to the front of the list.
    void pushFrontValue(T value) {
        if (size_ == 0) {
            head_ = new Node<T>(value);
            tail_ = head_;
            size_ = 1;
            return;
        }
        //Node<T>* previous_head = head_;
        Node<T>* new_head = new Node<T>(value);
        new_head->next = head_;
        head_ = new_head;
        ++size_;
    }

    // Prints all elements of the list in correct order.
    void printList() {
        // Needed for correct printing of spaces.
        bool was_something_printed = false;
        Node<T>* current_node = head_;
        for (int i = 0; i < size_; ++i) {
            if (was_something_printed) {
                std::cout << " ";
            }
            std::cout << current_node->value;
            current_node = current_node->next;
            was_something_printed = true;
        }
    }
};

// Rotates list.
List<int>* rotateList(List<int>* initial_list) {
    auto rotated_list = new List<int>();
    int length = initial_list->size();
    for (int i = 0; i < length; ++i) {
        rotated_list->pushFrontValue((initial_list->popFront())->value);
    }
    return rotated_list;
}

int main() {
    int length;
    std::cin >> length;
    auto list = new List<int>();
    list->fillList(length);
    auto rotated_list = rotateList(list);
    rotated_list->printList();
    delete list;
    delete rotated_list;
    return 0;
}
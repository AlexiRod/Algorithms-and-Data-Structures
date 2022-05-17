#include <iostream>

template <class KeyType, class ValueType>
struct Node {
    KeyType key_;
    ValueType value_;
    Node<KeyType, ValueType>* next_;

    Node(KeyType key, ValueType val) {
        key_ = key;
        value_ = val;
        next_ = nullptr;
    }

    void deleteNode() {
        if (!next_) {
            return;
        }

        next_->deleteNode();
        delete next_;
    }

    bool addNode(KeyType key, ValueType value) {
        if (key_ == key) {
            value_ = value;
            return false;
        }

        if (!next_) {
            return next_ = new Node<KeyType, ValueType>(key, value);
        } else {
            return next_->addNode(key, value);
        }
    }

    void setNextNode(Node* node) {
        if (next_) {
            next_->setNextNode(node);
        } else {
            next_ = node;
        }
    }

    ValueType* getValue(KeyType key) {
        if (key_ == key) {
            return &value_;
        }

        if (next_) {
            return next_->getValue(key);
        }
        return nullptr;
    }
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
private:
    Func hasher_;
    Node<KeyType, ValueType>** table_;
    size_t capacity_;
    size_t size_;
    double kof_;

    void rehash() {
        capacity_ *= 2;
        Node<KeyType, ValueType>** new_table = new Node<KeyType, ValueType>*[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            new_table[i] = nullptr;
        }

        for (size_t ind = 0; ind < capacity_ / 2; ++ind) {
            Node<KeyType, ValueType>* node = table_[ind];
            while (node) {
                size_t new_ind = hasher_(node->key_) % capacity_;

                if (!new_table[new_ind]) {
                    new_table[new_ind] = node;
                } else {
                    new_table[new_ind]->setNextNode(node);
                }

                Node<KeyType, ValueType>* tmp = node;
                node = node->next_;
                tmp->next_ = nullptr;
            }
        }
        delete[] table_;
        table_ = new_table;
    }

public:
    HashTable() {
        size_ = 0;
        capacity_ = 100;
        hasher_ = Func();
        kof_ = 0.5;

        table_ = new Node<KeyType, ValueType>*[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            table_[i] = nullptr;
        }
    }

    explicit HashTable(Func hasher) {
        size_ = 0;
        capacity_ = 100;
        hasher_ = hasher;
        kof_ = 0.5;

        table_ = new Node<KeyType, ValueType>*[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            table_[i] = nullptr;
        }
    }

    HashTable(size_t capacity, double kof, Func hasher = std::hash<KeyType>()) {
        size_ = 0;
        capacity_ = capacity;
        hasher_ = hasher;

        if (kof <= 0 || kof > 1) {
            kof_ = 0.5;
        } else {
            kof_ = kof;
        }

        table_ = new Node<KeyType, ValueType>*[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            table_[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i]) {
                table_[i]->deleteNode();
                delete table_[i];
            }
        }
        delete[] table_;
    }

    void insert(KeyType key, ValueType value) {
        size_t ind = hasher_(key) % capacity_;

        if (!table_[ind]) {
            table_[ind] = new Node<KeyType, ValueType>(key, value);
            size_++;
        } else if (table_[ind]->addNode(key, value)) {
            size_++;
        }

        if (kof_ * static_cast<double>(capacity_) < static_cast<double>(size_)) {
            rehash();
        }
    }

    ValueType* find(KeyType key) {
        size_t ind = hasher_(key) % capacity_;

        if (table_[ind]) {
            return table_[ind]->getValue(key);
        }
        return nullptr;
    }

    void erase(KeyType key) {
        size_t ind = hasher_(key) % capacity_;

        if (!table_[ind]) {
            return;
        }

        if (table_[ind]->key_ == key) {
            Node<KeyType, ValueType>* next = table_[ind]->next_;
            delete table_[ind];

            table_[ind] = next;
            --size_;
            return;
        }

        Node<KeyType, ValueType>* node = table_[ind]->next_;
        Node<KeyType, ValueType>* parent = table_[ind];

        while (node) {
            if (node->key_ == key) {
                parent->next_ = node->next_;
                delete node;
                --size_;
                return;
            }
            parent = node;
            node = node->next_;
        }
    }

    Node<KeyType, ValueType>& operator[](uint64_t ind) {
        if (ind < 0 || ind >= capacity_) {
            throw std::out_of_range("");
        }

        if (!table_[ind]) {
            throw std::runtime_error("");
        }

        return *table_[ind];
    }

    Node<KeyType, ValueType> at(uint64_t ind) {
        if (ind < 0 || ind >= capacity_) {
            throw std::out_of_range("");
        }

        if (!table_[ind]) {
            throw std::runtime_error("");
        }

        return *table_[ind];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }
};

int main() {
    HashTable<int, int>* ht = new HashTable<int, int>();
    ht->insert(1, 1);
    ht->insert(2, 2);
    ht->erase(2);
   
    return 0;
}




/* HashTable() {
     initialize(100, 0.5, Func());
 }

 explicit HashTable(Func fun) {
     initialize(100, 0.5, fun);
 }

 HashTable(size_t capacity, double kof, Func fun = std::hash<KeyType>()) {
     initialize(capacity, kof, fun);

     
     

    void initialize(size_t capacity, double kof, Func fun = std::hash<KeyType>()) {
        this->kof_ = kof > 0 && kof <= 1 ? kof_ : 0.5;
        this->capacity_ = capacity;
        hasher_ = fun;
        size_ = 0;

        table_ = new Node<KeyType, ValueType>*[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            table_[i] = nullptr;
        }
    }
     
     }*/

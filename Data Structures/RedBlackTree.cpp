#include <iostream>

enum Color { Black, Red };

template <class ValueType>
struct Node {
    ValueType value;
    Color color;
    Node *left, *right, *parent;

    Node() {
        left = right = parent = nullptr;
    }

    Node(ValueType val, Node* lptr, Node* rptr, Node* pptr, Color col) {
        value = val;
        left = lptr;
        right = rptr;
        parent = pptr;
        color = col;
    }

    ~Node() {
    }
};

template <class ValueType>
class RBTree {
private:
    Node<ValueType>* root_;
    int size_;

    int nodeTraversal(Node<ValueType>* node, ValueType* list, int ind) const {
        if (!node) {
            return 0;
        }
        if (node->left) {
            ind += nodeTraversal(node->left, list, ind);
        }
        list[ind] = node->value;
        if (node->right) {
            ind += nodeTraversal(node->right, list, ind + 1);
        }
        return ind + 1;
    }

    Node<ValueType>* copyNode(Node<ValueType>* node, Node<ValueType>* parent) {
        Node<ValueType>* copy_node = nullptr;
        if (node) {
            copy_node = new Node<ValueType>(node->value, copyNode(node->left),
                                            copyNode(node->right), parent, node->color);
        }
        return nullptr;
    }

    void deleteNode(Node<ValueType>* node) {
        if (node) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
            node = nullptr;
        }
    }

    bool isLeft(Node<ValueType>* child, Node<ValueType>* parent) {
        return child->value < parent->value;
    }

    Node<ValueType>* rightMin(Node<ValueType>* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    void changeChild(Node<ValueType>* parent, Node<ValueType>* child) {
        if (parent == root_) {
            root_ = child;
        } else if (parent == parent->parent->left) {
            parent->parent->left = child;
        } else {
            parent->parent->right = child;
        }

        if (child) {
            child->parent = parent->parent;
        }
    }

    //  <-----
    //    x
    //  A   y
    //    B   C
    void rotateLeft(Node<ValueType>* x) {
        Node<ValueType>* y = x->right;

        x->right = y->left;
        if (y->left) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent) {
            if (x == x->parent->left) {
                x->parent->left = y;
            } else {
                x->parent->right = y;
            }
        } else {
            root_ = y;
        }
        y->left = x;
        x->parent = y;
    }

    //  ----->
    //     y
    //   x   C
    // A   B
    void rotateRight(Node<ValueType>* y) {
        Node<ValueType>* x = y->left;

        y->left = x->right;
        if (x->right) {
            x->right->parent = y;
        }
        x->parent = y->parent;
        if (y->parent) {
            if (y == y->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
        } else {
            root_ = x;
        }

        x->right = y;
        y->parent = x;
    }

    void balanceAfterInsert(Node<ValueType>* node) {
        if (node == root_) {
            node->color = Color::Black;
            return;
        }

        Node<ValueType>* dad = node->parent;
        if (dad->color == Color::Red) {
            Node<ValueType>* grand = dad->parent;
            Node<ValueType>* unckle = isLeft(dad, grand) ? grand->right : grand->left;

            // 1. Есть красный дядя
            if (unckle && unckle->color == Color::Red) {
                unckle->color = Color::Black;
                dad->color = Color::Black;
                grand->color = Color::Red;
                balanceAfterInsert(grand);
            } else {
                if (isLeft(dad, grand)) {      // 2. Правый поворот деда
                    if (!isLeft(node, dad)) {  // 2.2 Вершина справа
                        rotateLeft(dad);
                        node->color = Color::Black;
                    } else {
                        // 2.1 Вершина слева
                        dad->color = Color::Black;
                    }

                    grand->color = Color::Red;
                    rotateRight(grand);
                } else {                      // 3. Левый поворот деда
                    if (isLeft(node, dad)) {  // 3.2 Вершина слева
                        rotateRight(dad);
                        node->color = Color::Black;
                    } else {
                        // 3.1 Вершина справа
                        dad->color = Color::Black;
                    }

                    grand->color = Color::Red;
                    rotateLeft(grand);
                }
            }
        }
        root_->color = Color::Black;
    }

    void balanceAfterErase(Node<ValueType>* p) {
        if (!p) {
            return;
        }

        while (p->color == Color::Black && p != root_) {
            Node<ValueType>* brother;
            Node<ValueType>* parent = p->parent;

            if (isLeft(p, parent)) {
                brother = p->right;
                if (brother->color == Color::Red) {  // 1. Брат красный
                    brother->color = Color::Black;
                    parent->color = Color::Red;
                    rotateLeft(parent);
                }

                // 2. Черные дети
                if ((!brother->left || brother->left->color == Color::Black) &&
                    (!brother->right || brother->right->color == Color::Black)) {
                    brother->color = Color::Red;
                    p = parent;
                } else {
                    if (brother->right->color == Color::Black) {
                        if (brother->left) {
                            brother->left->color = Color::Black;
                        }
                        brother->color = Color::Red;

                        rotateRight(brother);
                        brother = parent->right;
                    }

                    // 3. Красный и черный ребенок
                    brother->color = parent->color;
                    parent->color = Color::Black;
                    brother->right->color = Color::Black;

                    rotateLeft(parent);
                    p = root_;
                }
            } else {
                brother = p->left;
                if (brother->color == Color::Red) {  // 1. Брат красный
                    brother->color = Color::Black;
                    parent->color = Color::Red;
                    rotateRight(parent);
                }

                // 2. Черные дети
                if ((!brother->left || brother->left->color == Color::Black) &&
                    (!brother->right || brother->right->color == Color::Black)) {
                    brother->color = Color::Red;
                    p = parent;
                } else {
                    if (brother->left->color == Color::Black) {
                        if (brother->right) {
                            brother->right->color = Color::Black;
                        }
                        brother->color = Color::Red;

                        rotateLeft(brother);
                        brother = parent->left;
                    }

                    // 3. Красный и черный ребенок
                    brother->color = parent->color;
                    parent->color = Color::Black;
                    brother->left->color = Color::Black;

                    rotateRight(parent);
                    p = root_;
                }
            }
        }
        p->color = Color::Black;
        root_->color = Color::Black;
    }

public:
    RBTree() {
        root_ = nullptr;
        size_ = 0;
    }

    RBTree(const RBTree& other) {
        root_ = copyNode(other.root_, nullptr);
        size_ = other.size_;
    }

    RBTree& operator=(const RBTree& other) {
        if (this != other) {
            deleteNode(root_);
            root_ = copyNode(other.root_, nullptr);
            size_ = other.size_;
        }

        return *this;
    }

    ~RBTree() {
        deleteNode(root_);
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ <= 0;
    }

    void insert(const ValueType& value) {
        Node<ValueType>* node = root_;
        Node<ValueType>* parent = nullptr;

        if (!root_) {
            root_ = new Node<ValueType>(value, nullptr, nullptr, nullptr, Color::Black);
            size_++;
            return;
        }

        while (node) {
            if (node->value == value) {
                return;
            }
            parent = node;
            node = value < node->value ? node->left : node->right;
        }

        if (value < parent->value) {
            parent->left = new Node<ValueType>(value, nullptr, nullptr, parent, Color::Red);
            balanceAfterInsert(parent->left);
        } else {
            parent->right = new Node<ValueType>(value, nullptr, nullptr, parent, Color::Red);
            balanceAfterInsert(parent->right);
        }
        size_++;
    }

    void erase(const ValueType& value) {
        Node<ValueType>* z = root_;
        while (z->value != value) {
            z = z->value < value ? z->right : z->left;
            if (!z) {
                return;
            }
        }

        size_--;

        Node<ValueType>* y = z;
        Node<ValueType>* x = nullptr;
        auto y_color = y->color;

        if (!z->left && !z->right) {
            if (isLeft(z, z->parent)) {
                z->parent->left = nullptr;
            } else {
                z->parent->right = nullptr;
            }
        } else if (z->left && !z->right) {
            x = z->right;
            changeChild(z, z->right);
        } else if (z->right && !z->left) {
            x = z->left;
            changeChild(z, z->left);
        } else {
            y = rightMin(z->right);
            x = y->right;
            y_color = y->color;

            if (z->right == y) {
                if (x) {
                    x->parent = z;
                }
            } else {
                changeChild(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            changeChild(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        if (y_color == Black) {
            balanceAfterErase(x);
        }
    }

    ValueType* lower_bound(const ValueType& value) const {  // NOLINT
        Node<ValueType>* node = root_;
        ValueType* bound = nullptr;
        while (node) {
            if (node->value < value) {
                node = node->right;
            } else {
                bound = node->value;
                node = node->left;
            }
        }

        return bound;
    }

    ValueType* find(const ValueType& value) const {
        Node<ValueType>* node = root_;
        while (node) {
            if (node->value == value) {
                return node->value;
            }
            node = value < node->value ? node->left : node->right;
        }
        return nullptr;
    }

    ValueType* traversal() const {
        if (empty()) {
            return nullptr;
        }

        ValueType* list = new ValueType[size_];
        nodeTraversal(root_, list, 0);
        return list;
    }
};

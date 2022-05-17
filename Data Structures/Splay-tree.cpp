#include <iostream>

struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(int key) {
        this->key = key;
        left = right = parent = nullptr;
    }
};

class SplayTree {
    Node* root_;

    void deleteNode(Node* node) {
        if (node) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
            node = nullptr;
        }
    }

    void leftRotate(Node* node) {
        Node* parent = node->parent;
        Node* right = node->right;

        if (parent) {
            if (node == parent->left) {
                parent->left = right;
            } else {
                parent->right = right;
            }
        }

        Node* sib = right->left;
        right->left = node;

        node->right = sib;
        node->parent = right;
        right->parent = parent;

        if (node->right) {
            node->right->parent = node;
        }
    }

    void rightRotate(Node* node) {
        Node* parent = node->parent;
        Node* left = node->left;

        if (parent) {
            if (node == parent->left) {
                parent->left = left;
            } else {
                parent->right = left;
            }
        }

        Node* sib = left->right;
        left->right = node;

        node->left = sib;
        node->parent = left;
        left->parent = parent;

        if (node->left) {
            node->left->parent = node;
        }
    }

    int getNodeHeight(Node* node) const {
        if (!node || !node->left && !node->right) {
            return 0;
        }

        int left_height = getNodeHeight(node->left) + 1;
        int right_height = getNodeHeight(node->right) + 1;
        return left_height > right_height ? left_height : right_height;
    }

    bool isLeft(Node* node, Node* parent) {
        return parent->left == node;
    }

public:
    SplayTree() {
        root_ = nullptr;
    }

    ~SplayTree() {
        deleteNode(root_);
    }

    void insert(int key) {
        Node* node = root_;
        Node* parent = nullptr;

        if (!root_) {
            root_ = new Node(key);
            return;
        }

        while (node) {
            if (node->key == key) {
                return;
            }
            parent = node;
            node = key > node->key ? node->right : node->left;
        }

        if (key > parent->key) {
            parent->right = new Node(key);
            parent->right->parent = parent;
        } else {
            parent->left = new Node(key);
            parent->left->parent = parent;
        }
    }

    Node* find(int key) const {
        Node* node = root_;

        while (node) {
            if (node->key == key) {
                return node;
            }
            node = key > node->key ? node->right : node->left;
        }
        return nullptr;
    }

    int splay(Node* node) {
        if (!node) {
            return 0;
        }

        int count = 0;
        while (node->parent) {
            if (isLeft(node, node->parent)) {
                if (!node->parent->parent) {
                    ++count;
                    rightRotate(node->parent);
                } else if (isLeft(node->parent, node->parent->parent)) {
                    count += 2;
                    rightRotate(node->parent->parent);
                    rightRotate(node->parent);
                } else {
                    ++count;
                    rightRotate(node->parent);
                    leftRotate(node->parent);
                }
            } else {
                if (!node->parent->parent) {
                    ++count;
                    leftRotate(node->parent);
                } else if (!isLeft(node->parent, node->parent->parent)) {
                    count += 2;
                    leftRotate(node->parent->parent);
                    leftRotate(node->parent);
                } else {
                    ++count;
                    leftRotate(node->parent);
                    rightRotate(node->parent);
                }
            }
        }
        root_ = node;
        return count;
    }

    int getHeight() {
        return getNodeHeight(root_);
    }
};

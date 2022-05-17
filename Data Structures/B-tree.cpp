#include <iostream>

struct Node {
public:
    int t_, length;
    int* keys;
    Node** childs;
    bool leaf;

    Node(int t_con, bool is_leaf) {
        t_ = t_con;
        leaf = is_leaf;
        length = 0;

        keys = new int[2 * t_ - 1];
        childs = new Node*[2 * t_];

        for (int i = 0; i < 2 * t_ - 1; ++i) {
            childs[i] = nullptr;
            keys[i] = 0;
        }
        childs[2 * t_ - 1] = nullptr;
    }

    void deleteNode() {
        if (!leaf) {
            for (int i = 0; i <= length; ++i) {
                if (childs[i]) {
                    childs[i]->deleteNode();
                }
            }
        }

        delete[] keys;
        delete[] childs;
        delete this;
    }

    void insertToNode(int key) {
        int ind = length - 1;

        for (int i = 0; i < length; ++i) {
            if (keys[i] == key) {
                return;
            }
        }

        while (ind >= 0 && keys[ind] > key) {
            --ind;
        }

        if (ind >= 0 && keys[ind] == key) {
            return;
        }

        if (leaf) {
            ind = length - 1;

            while (ind >= 0 && keys[ind] > key) {
                keys[ind + 1] = keys[ind];
                --ind;
            }

            keys[ind + 1] = key;
            length++;
        } else {
            for (int i = 0; i < childs[ind + 1]->length; ++i) {
                if (childs[ind + 1]->keys[i] == key) {
                    return;
                }
            }

            if (childs[ind + 1]->length == 2 * t_ - 1) {
                splitNode(ind + 1, childs[ind + 1]);
                if (keys[ind + 1] < key) {
                    ++ind;
                }
            }

            childs[ind + 1]->insertToNode(key);
        }
    }

    void splitNode(int ind, Node* node) {
        Node* child = new Node(node->t_, node->leaf);
        child->length = t_ - 1;
        node->length = t_ - 1;

        for (int i = 0; i < t_ - 1; ++i) {
            child->keys[i] = node->keys[i + t_];
        }
        if (!node->leaf) {
            for (int i = 0; i < t_; ++i) {
                child->childs[i] = node->childs[i + t_];
            }
        }

        for (int i = length; i >= ind + 1; --i) {
            childs[i + 1] = childs[i];
        }
        childs[ind + 1] = child;

        for (int i = length - 1; i >= ind; --i) {
            keys[i + 1] = keys[i];
        }

        keys[ind] = node->keys[t_ - 1];
        length++;
    }
};

class BTree {
    Node* root_;
    int t_;

    size_t getSize(Node* node) const {
        if (!node) {
            return 0;
        }
        if (node->leaf) {
            return 1;
        }
        size_t sum = 1;
        for (int i = 0; i < node->t_ * 2; ++i) {
            sum += getSize(node->childs[i]);
        }
        return sum;
    }

    int64_t getLeafSum(Node* node) const {
        if (!node) {
            return 0;
        }

        int64_t sum = 0;
        if (!node->leaf) {
            for (int i = 0; i <= node->length; ++i) {
                sum += getLeafSum(node->childs[i]);
            }
        } else {
            for (int i = 0; i < node->length; ++i) {
                sum += node->keys[i];
            }
        }
        return sum;
    }

public:
    explicit BTree(int t_con) {
        t_ = t_con;
        root_ = nullptr;
    }

    ~BTree() {
        if (root_) {
            root_->deleteNode();
        }
    }

    void insert(int key) {
        if (!root_) {
            root_ = new Node(t_, true);
            root_->keys[0] = key;
            root_->length = 1;
            return;
        }

        if (root_->length != 2 * t_ - 1) {
            root_->insertToNode(key);
            return;
        }

        for (int i = 0; i < root_->length; ++i) {
            if (root_->keys[i] == key) {
                return;
            }
        }

        Node* node = new Node(t_, false);
        node->childs[0] = root_;
        node->splitNode(0, root_);
        node->childs[node->keys[0] < key ? 1 : 0]->insertToNode(key);

        root_ = node;
    }

    size_t size() const {
        return getSize(root_);
    }

    int64_t sum() const {
        return getLeafSum(root_);
    }
};

int main() {


    BTree t_(2);

    t_.insert(1);
    t_.insert(2);
    t_.insert(3);
    t_.insert(4);
    t_.insert(5);
    t_.insert(6);
    t_.insert(7);
    t_.insert(8);
    t_.insert(9);

    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";

    t_.insert(7);

    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";

    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(1);
    t_.insert(2);
    t_.insert(3);
    t_.insert(3);
    t_.insert(3);
    t_.insert(4);
    t_.insert(5);
    t_.insert(5);
    t_.insert(6);
    t_.insert(7);
    t_.insert(7);
    t_.insert(7);

    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";

    t_.insert(8);
    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";
    t_.insert(9);
    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";
    t_.insert(10);

    std::cout << t_.size() << "\n";
    std::cout << t_.sum() << "\n";

    return 0;
}











/*




struct BNode {
    int* keys;
    BNode** children;
    BNode* parent;
    int count;
    int countSons;
    bool leaf;
    int t_;

    BNode(int t_con, bool is_leaf) {
        keys = new int[2 * t_con];
        children = new BNode * [2 * t_con + 1];

        for (int i = 0; i < 2 * t_con; ++i) {
            keys[i] = kMax;
            children[i] = nullptr;
        }
        children[2 * t_con] = nullptr;

        parent = nullptr;
        count = countSons = 0;
        leaf = is_leaf;

        t_ = t_con;
    }

    void deleteNode() {
        if (!leaf) {
            for (int i = 0; i <= countSons; ++i) {
                if (children[i]) {
                    children[i]->deleteNode();
                }
            }
        }
        delete[] keys;
        delete[] children;
        delete this;
    }
};

class BTree {
private:
    int t_;
    size_t size_;
    BNode* root_;

    void insertKeyToNode(int key, BNode* node) {
        node->count++;
        for (int i = 0; i < 2 * t_; ++i) {
            if (node->keys[i] == key) {
                return;
            }
            if (node->keys[i] > key) {
                for (int j = 2 * t_ - 1; j > i; --j) {
                    node->keys[j] = node->keys[j - 1];
                }
                node->keys[i] = key;
                break;
            }
        }
    }

    void checkSplit(BNode* node) {
        if (!node || node->count < 2 * t_) {
            return;
        }

        BNode* left_child = new BNode(t_, false);
        for (int ind = 0; ind < t_ - 1; ++ind) {
            left_child->keys[ind] = node->keys[ind];
            left_child->children[ind] = node->children[ind];

            if (left_child->keys[ind] != kMax) {
                left_child->count++;
            }

            if (left_child->children[ind]) {
                left_child->children[ind]->parent = left_child;
                left_child->countSons++;
            }
        }
        left_child->leaf = node->leaf;
        left_child->children[t_ - 1] = node->children[t_ - 1];
        if (left_child->children[t_ - 1]) {
            left_child->children[t_ - 1]->parent = left_child;
            left_child->countSons++;
        }

        BNode* right_child = new BNode(t_, false);
        for (int ind = 0; ind < t_; ++ind) {
            right_child->keys[ind] = node->keys[t_ + ind];
            right_child->children[ind] = node->children[t_ + ind];

            if (right_child->keys[ind] != kMax) {
                right_child->count++;
            }

            if (right_child->children[ind]) {
                right_child->children[ind]->parent = right_child;
                right_child->countSons++;
            }
        }
        right_child->leaf = node->leaf;
        right_child->children[t_] = node->children[2 * t_];
        if (right_child->children[t_]) {
            right_child->children[t_]->parent = right_child;
            right_child->countSons++;
        }

        size_++;
        if (node == root_) {
            size_++;
            node->keys[0] = node->keys[t_ - 1];
            for (int i = 1; i < 2 * t_; ++i) {
                node->keys[i] = kMax;
            }

            node->children[0] = left_child;
            node->children[1] = right_child;
            left_child->parent = node;
            right_child->parent = node;
            for (int i = 2; i < 2 * t_ + 1; ++i) {
                node->children[i] = nullptr;
            }

            node->parent = nullptr;
            node->leaf = false;
            node->count = 1;
            node->countSons = 2;
            return;
        }

        BNode* parent = node->parent;
        insertKeyToNode(node->keys[t_ - 1], parent);

        for (int i = 0; i < 2 * t_ + 1; ++i) {
            if (parent->children[i] == node) {
                for (int j = 2 * t_; j > i + 1; --j) {
                    parent->children[j] = parent->children[j - 1];
                }

                parent->children[i + 1] = right_child;
                parent->children[i] = left_child;
                break;
            }
        }

        parent->countSons++;
        parent->leaf = false;
        left_child->parent = parent;
        right_child->parent = parent;
        node->deleteNode();
        checkSplit(parent);
    }

    int64_t getLeafSum(BNode* node) const {
        int64_t sum = 0;

        if (!node) {
            return 0;
        }

        if (!node->leaf) {
            for (int i = 0; i < node->countSons; ++i) {
                sum += getLeafSum(node->children[i]);
            }
            return sum;
        }

        for (int i = 0; i < node->count; ++i) {
            sum += node->keys[i];
        }
        return sum;
    }

public:
    explicit BTree(int t_con) {
        this->t_ = t_con;
        size_ = 0;
        root_ = nullptr;
    }

    ~BTree() {
        if (root_) {
            root_->deleteNode();
        }
    }

    void insert(int key) {
        if (!root_) {
            root_ = new BNode(t_, true);
            root_->keys[0] = key;
            root_->count++;
            size_++;
            return;
        }

        BNode* node = root_;
        while (!node->leaf) {
            for (int ind = 0; ind < 2 * t_ - 1; ++ind) {
                if (node->keys[ind] == kMax) {
                    node = node->children[ind];
                    break;
                }

                if (key < node->keys[ind]) {
                    node = node->children[ind];
                    break;
                }
                if (ind == 2 * t_ - 2) {
                    node = node->children[ind + 1];
                }
            }
        }
        for (int i = 0; i < node->count; ++i) {
            if (node->keys[i] == key) {
                return;
            }
        }

        insertKeyToNode(key, node);
        checkSplit(node);
    }

    size_t size() const {
        return size_;
    }

    int64_t sum() const {
        return getLeafSum(root_);
    }
};




    void print(char delm = '\0') {
        for (int i = 0; i < 2 * t_ - 1; i++) {
            if (keys[i] != kMax) {
                std::cout << keys[i] << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << "[" << count << ";" << countSons << "]" << delm;
    }


    void printLevel(std::vector<BNode*> nodes, int ind = 1) {
        if (!nodes.size()) {
            return;
        }

        std::vector<BNode*> v;
        std::cout << "\n" << ind++ << ") ";


        for (int i = 0; i < nodes.size(); ++i) {
            if (!nodes[i]) {
                std::cout << "n   ";
                continue;
            }

            nodes[i]->print();
            std::cout << "   ";

            if (nodes[i]->leaf) {
                continue;
            }

            for (int j = 0; j < 2 * t; ++j) {
                v.push_back(nodes[i]->children[j]);
            }
        }
        printLevel(v, ind);
    }

    void print() {
        //std::cout << "1) ";
        //root->print();
        std::vector<BNode*> v = { root };
        printLevel(v);
        std::cout << "\n";
    }







    

int main(int a, char c) {
    BTree tree(2);

    tree.insert(7);
    tree.insert(2);
    tree.insert(8);
    tree.insert(1);
    tree.insert(7);
    tree.insert(9);
    tree.insert(9);

    std::cout << tree.size() << "\n";
    std::cout << tree.sum() << "\n";

    tree.insert(7);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(9);

    std::cout << tree.size() << "\n";
    std::cout << tree.sum() << "\n";
}


*/




//tree.insert(15);
//tree.insert(16);
//tree.insert(17);
//tree.insert(18);
////tree.print();
//tree.insert(11);
//tree.insert(12);
//tree.insert(13);
//tree.insert(14);
//tree.print();
//
//tree.insert(19);
//tree.insert(20);
//tree.insert(17);
//tree.insert(15);
//tree.insert(17);
//tree.print();
//tree.insert(17);
//tree.print();
//tree.insert(15);
//tree.print();
//tree.insert(15);
//tree.print();



/*
            insertKeyToNode(7, root);
            root->print();
            insertKeyToNode(1, root);
            root->print();
            insertKeyToNode(4, root);
            root->print();
            insertKeyToNode(5, root);
            root->print();
            insertKeyToNode(3, root);
            root->print();
            insertKeyToNode(2, root);
            root->print();
*/
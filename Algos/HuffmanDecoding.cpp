#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using std::string;

struct Node {
    char symb;
    int freq;
    Node* left, * right;

    Node(char symb, int freq, Node* left, Node* right) {
        this->symb = symb;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }
};

struct FrequencyComparator {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class DecodeTree {
    Node* root_ = new Node('\0', 1, nullptr, nullptr);

    void addCode(Node* node, const char& symb, const string& code, int index) {
        if (index == code.size() - 1) {
            if (code[index] == '0') {
                node->left = new Node(symb, 1, nullptr, nullptr);
            }
            else {
                node->right = new Node(symb, 1, nullptr, nullptr);
            }
            return;
        }
        if (code[index] == '0') {
            if (node->left == nullptr) {
                node->left = new Node('\0', 1, nullptr, nullptr);
            }
            addCode(node->left, symb, code, index + 1);
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node('\0', 1, nullptr, nullptr);
            }
            addCode(node->right, symb, code, index + 1);
        }
    }

    void deleteNode(Node* node) {
        if (node) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    void add(const char& symb, const string& code) {
        addCode(root_, symb, code, 0);
    }

    Node* getRoot() {
        return root_;
    }

    char getSymb(char symb) {
        if (symb == '1') {
            return root_->right->symb;
        }
        return root_->left->symb;
    }
};

class HuffmanCoder {
    std::unordered_map<char, string> codes_;
    Node* root_ = nullptr;

    void encode(Node* node, const string& str) {
        if (node == nullptr) {
            return;
        }
        if (!node->left && !node->right) {
            codes_[node->symb] = str;
        }
        encode(node->left, str + "0");
        encode(node->right, str + "1");
    }

    void decode(Node* node, int* index, const string& str) {
        if (node == nullptr) {
            return;
        }
        if (!node->left && !node->right) {
            std::cout << node->symb;
            return;
        }
        ++* index;
        if (*index >= static_cast<int>(str.size())) {
            return;
        }
        if (str[*index] == '0') {
            decode(node->left, index, str);
        }
        else {
            decode(node->right, index, str);
        }
    }

    void buildTree(string str) {
        std::unordered_map<char, int> freqs;
        for (char symb : str) {
            freqs[symb]++;
        }

        std::priority_queue<Node*, std::vector<Node*>, FrequencyComparator> huffman_queue;
        for (auto pair : freqs) {
            huffman_queue.push(new Node(pair.first, pair.second, nullptr, nullptr));
        }

        if (huffman_queue.size() == 1) {
            codes_[str[0]] = "0";
            root_ = huffman_queue.top();
            return;
        }

        while (huffman_queue.size() != 1) {
            Node* left = huffman_queue.top();
            huffman_queue.pop();
            Node* right = huffman_queue.top();
            huffman_queue.pop();
            int sum = left->freq + right->freq;
            huffman_queue.push(new Node('\0', sum, left, right));
        }

        root_ = huffman_queue.top();
        encode(root_, "");
    }

    void deleteNode(Node* node) {
        if (node) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    void printEnocdedInfo(string str) {
        buildTree(str);

        string encoded;
        for (char symb : str) {
            encoded += codes_[symb];
        }

        std::cout << codes_.size() << " " << encoded.size() << "\n";
        for (auto pair : codes_) {
            std::cout << pair.first << ": " << pair.second << "\n";
        }
        std::cout << encoded << "\n";
    }

    void printDecodedInfo() {
        int size, length;
        std::cin >> size >> length;

        char symb;
        string code, str;
        DecodeTree tree;
        for (int i = 0; i < size; ++i) {
            std::cin >> symb >> code >> code;
            tree.add(symb, code);
        }
        root_ = tree.getRoot();

        std::cin >> str;
        if (str.size() == 1) {
            std::cout << tree.getSymb(str[0]);
            return;
        }
        if (str.size() == 2) {
            std::cout << tree.getSymb(str[0]) << tree.getSymb(str[1]);
            return;
        }

        int index = -1;
        while (index < length) {
            decode(root_, &index, str);
        }
    }

    ~HuffmanCoder() {
        deleteNode(root_);
    }
};

int main() {
    HuffmanCoder coder;
    coder.printDecodedInfo();
}

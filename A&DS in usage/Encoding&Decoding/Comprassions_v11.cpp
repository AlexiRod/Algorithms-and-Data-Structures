#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <map>
#include<windows.h>

using namespace std;

class RLCoder
{
    int stringLength = 0;
    int resLength = 0;
public:

    string RLE1(string a)
    {
        stringLength = a.length();
        char symb = a[0];
        int count = 0;
        string res = "";

        for (int i = 0; i < a.size(); i++)
        {
            if (a[i] == symb)
            {
                count++;
                continue;
            }

            res.append(to_string(count) + symb);
            symb = a[i];
            count = 1;
        }
        res.append(to_string(count) + symb);
        resLength = res.length();
        return res;
    }


    string RLE2(string a)
    {
        stringLength = a.length();
        char symb = a[0];
        int count = 1;
        int len = 1;
        string res = "";
        string notrepeated = "";
        notrepeated.push_back(a[0]);
        bool isRepeated = false;

        for (int i = 1; i < a.size(); i++)
        {
            if (a[i] == symb)
            {
                count++;
                isRepeated = true;
                notrepeated = "";
                continue;
            }

            if (isRepeated)
            {
                res.append(to_string(count) + symb);
                count = 1;
            }
            symb = a[i];

            if (a[i + 1] != a[i])
            {
                notrepeated.push_back(a[i]);
                len++;
                isRepeated = false;
            }
            else
            {
                if (isRepeated)
                    continue;

                res.append(to_string(-len) + notrepeated);
                notrepeated = "";
                len = 0;
                symb = a[i];
                isRepeated = true;
            }
        }

        if (len == 0)
            res.append(to_string(count) + symb);
        else  res.append(to_string(-len) + notrepeated);
        resLength = res.length();
        return res;
    }

    string getKof() 
    {
        return "    Коэффициент сжатия: " + to_string(resLength) + " / " + to_string(stringLength) + 
            " = " + to_string((double)resLength / stringLength);
    }
};



class LZCoder 
{
private:
    int stringLength = 0;
    int resLength = 0;

    struct Node {
        int pos;
        char next;
    };

public:
    string LZ77(string s, int len) {
        stringLength = s.length();
        resLength = 0;
        string res;
        deque<char> buffer;

        for (int i = 0; i < s.size(); i++)
        {
            int offset = 0, length = 0;
            for (int j = 0; j < buffer.size(); j++)
            {
                if (buffer[j] == s[i])
                {
                    offset = buffer.size() - j;
                    while (j < buffer.size() && buffer[j] == s[i])
                    {
                        length++;
                        i++;
                        j++;
                    }

                    res += " <" + to_string(offset) + "|" + to_string(length) + "|" + s[i] + "> ";
                    resLength += to_string(offset).length() + to_string(length).length() + 3;
                    break;
                }
            }

            if (!length) 
            {
                res += s[i];
                resLength++;
            }

            if (buffer.size() >= len)
            {
                buffer.push_back(s[i]);
                buffer.pop_front();
            }
            else
                buffer.push_back(s[i]);
        }
        return res;
    }

    string LZ78(string s)
    {
        stringLength = s.length();
        resLength = 0;
        string buffer = "";
        map<string, int> dict = {};
        vector<Node> ans;

        for (int i = 0; i < s.size(); i++)
        {
            if (dict.find(buffer + s[i]) != dict.end())
                buffer += s[i];
            else
            {
                ans.push_back({ dict[buffer], s[i] });
                dict[buffer + s[i]] = dict.size();
                buffer = "";
            }
        }

        if (!buffer.empty())
        {
            char last_ch = buffer[buffer.size() - 1];
            string s = buffer.substr(0, buffer.size() - 1);
            ans.push_back({ dict[s], last_ch });
        }

        string res = "";
        for (int i = 0; i < ans.size(); i++) 
        {
            res.append("<" + to_string(ans[i].pos) + "," + ans[i].next + "> ");
            resLength += to_string(ans[i].pos).length() + 1 + 2;
        }
        return res;
    }

    string getKof()
    {
        return "    Коэффициент сжатия: " + to_string(resLength) + " / " + to_string(stringLength) +
            " = " + to_string((double)resLength / stringLength);
    }
};



class ShennonFannoCoder
{
    struct node
    {
        char symb;
        double prob;
    };

    static int CompareProb(const void* fitst, const void* second)
    {
        node a = *(node*)fitst;
        node b = *(node*)second;
        return b.prob - a.prob;
    }

private:
    int maxBit = 0;
    int length = 0;
    int size;
    node* nodes;
    map<char, string> codes_;
    map<string, char> decodes;



    void SchennonFanno(int left, int right)
    {
        if (left >= right)
            return;

        if (right - left == 1)
        {
            codes_[nodes[left].symb] += '0';
            codes_[nodes[right].symb] += '1';
            return;
        }

        double sum = 0;
        for (int i = left; i <= right; ++i)
            sum += nodes[i].prob;

        int splitIndex = -1;
        double prob = 0;
        for (int i = left; i <= right; ++i)
        {
            prob += nodes[i].prob;
            if (prob > sum * 0.5)
            {
                codes_[nodes[i].symb] += '1';
                if (splitIndex < 0)
                    splitIndex = i;
                continue;
            }
            codes_[nodes[i].symb] += '0';
        }

        if (splitIndex < 0 || splitIndex == left)
            splitIndex = left + 1;

        SchennonFanno(left, splitIndex - 1);
        SchennonFanno(splitIndex, right);
    }

public:
    string EncodeString(string s)
    {
        map<char, int> frequencies;
        for (int i = 0; i < s.size(); i++)
            frequencies[s[i]]++;

        size = frequencies.size();
        length = s.size();
        nodes = new node[size];

        map<char, int>::iterator freqIterator = frequencies.begin();
        for (int i = 0; i < size; freqIterator++, i++)
            nodes[i] = { (*freqIterator).first, double((*freqIterator).second) / (double)s.size() };
        qsort(nodes, size, sizeof(node), CompareProb);

        SchennonFanno(0, size - 1);

        cout << "Таблица вероятоностей для букв:\n";
        for (int i = 0; i < size; i++)
            cout << nodes[i].symb << " " << nodes[i].prob << "\n";

        cout << "\nЗакодированные буквы:\n";
        map<char, string>::iterator codeIterator = codes_.begin();
        for (; codeIterator != codes_.end(); codeIterator++)
        {
            cout << (*codeIterator).first << " " << (*codeIterator).second << "\n";
            decodes[(*codeIterator).second] = (*codeIterator).first;
            maxBit = max(maxBit, ((*codeIterator).second).length());
        }

        string res = to_string(s.size()) + " ";
        for (int i = 0; i < s.size(); i++)
            res += codes_[s[i]] + " ";
        cout << "\nЗакодированный текст:\n" << res << "\n";
        return res;
    }

    void PrintDecodeString(string s)
    {
        stringstream stream(s);
        string word;
        stream >> word;
        int len = stoi(word);
        cout << "\nДекодированный текст:\n";
        for (int i = 0; i < len; i++)
        {
            stream >> word;
            cout << decodes[word];
        }
        cout << "\n\nКоэффициент сжатия: " << length << " * " << maxBit << " / " << length << " * 8 = " << double(maxBit * length) / (length * 8) << "\n";
    }
};


class HuffmanCoder
{
    struct Node
    {
        char symb;
        double prob;
        string code;
        Node* left, *right;

        Node() {}

        Node(char s, double p, string c)
        {
            left  = nullptr;
            right = nullptr;
            symb = s;
            prob = p;
            code = c;
        }
    };

    static int CompareProb(const void* fitst, const void* second)
    {
        Node a = *(Node*)fitst;
        Node b = *(Node*)second;
        return b.prob - a.prob;
    }

private:
    int sumBit = 0;
    int length = 0;
    int size;
    Node* nodes;
    map<char, string> codes_;
    map<string, char> decodes;

    class HuffmanTree {
        Node* root_ = nullptr;

        void traverse(Node* node) 
        {
            if (!node)
                return;
            traverse(node->left);
            cout << node->symb << " (" << node->prob << ") = " << node->code << "\n";
            traverse(node->right);
        }

    public:
        void Insert(char symb, double prob, string code) {
            Node* node = root_;
            Node* parent = nullptr;

            if (root_ == nullptr)
            {
                root_ = new Node(symb, prob, code);
                return;
            }

            while (node != nullptr) {
                parent = node;
                node = prob >= node->prob ? node->right : node->left;
            }

            if (prob >= parent->prob)
                parent->right = new Node(symb, prob, code);
            else 
                parent->left = new Node(symb, prob, code);
        }

        void PrintTraverse() 
        {
            traverse(root_);
        }
    };

    void Huffman(int left, int right)
    {
        if (left >= right)
            return;

        if (right - left == 1)
        {
            codes_[nodes[left].symb] += '0';
            codes_[nodes[right].symb] += '1';
            return;
        }

        double sum = 0;
        for (int i = left; i <= right; ++i)
            sum += nodes[i].prob;

        int splitIndex = -1;
        double prob = 0;
        for (int i = left; i <= right; ++i)
        {
            prob += nodes[i].prob;
            if (prob > sum * 0.5)
            {
                codes_[nodes[i].symb] += '1';
                if (splitIndex < 0)
                    splitIndex = i;
                continue;
            }
            codes_[nodes[i].symb] += '0';
        }

        if (splitIndex < 0 || splitIndex == left)
            splitIndex = left + 1;

        Huffman(left, splitIndex - 1);
        Huffman(splitIndex, right);
    }

public:
    string EncodeString(string s)
    {
        map<char, int> frequencies;
        for (int i = 0; i < s.size(); i++)
            frequencies[s[i]]++;

        size = frequencies.size();
        length = s.size();
        nodes = new Node[size];

        map<char, int>::iterator freqIterator = frequencies.begin();
        for (int i = 0; i < size; freqIterator++, i++)
            nodes[i] = Node((*freqIterator).first, double((*freqIterator).second) / (double)s.size(), "");
        qsort(nodes, size, sizeof(Node), CompareProb);

        Huffman(0, size - 1);

        HuffmanTree tree;
        for (int i = 0; i < size; i++)
        {
            tree.Insert(nodes[i].symb, nodes[i].prob, codes_[nodes[i].symb]);
        }

        cout << "Дерево Хафмана с буквами, вероятностями и кодами (симметричный обход):\n";
        tree.PrintTraverse();

        map<char, string>::iterator codeIterator = codes_.begin();
        for (; codeIterator != codes_.end(); codeIterator++)
        {
            char symb = (*codeIterator).first;
            string code = (*codeIterator).second;
            decodes[code] = symb;
            sumBit += code.length();
        }

        string res = to_string(s.size()) + " ";
        for (int i = 0; i < s.size(); i++)
            res += codes_[s[i]] + " ";
        cout << "\nЗакодированный текст:\n" << res << "\n";
        return res;
    }

    void PrintDecodeString(string s)
    {
        stringstream stream(s);
        string word;
        stream >> word;
        int len = stoi(word);
        cout << "\nДекодированный текст:\n";
        for (int i = 0; i < len; i++)
        {
            stream >> word;
            cout << decodes[word];
        }
        cout << "\n\nКоэффициент сжатия: " << sumBit << " / " << length << " * 8 = " << double(sumBit) / (length * 8) << "\n";
    }

};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    RLCoder rlcoder;
    cout << "Метод сжатия RLE без группового кодирования:\n";
    cout << rlcoder.RLE1("aaaaaaaaaaabbbbbbbbbaaaaacccaaaba");
    cout << rlcoder.getKof() << "\n";
    cout << rlcoder.RLE1("abcdfwnpnskenfinvpda");
    cout << rlcoder.getKof() << "\n";

    cout << "\n\n---------\n\nМетод сжатия RLE с групповым кодирования:\n";
    cout << rlcoder.RLE2("aaaaaaaaaaabbbbbbbbbaaaaacccaaaba");
    cout << rlcoder.getKof() << "\n";
    cout << rlcoder.RLE2("abcdfwnpnskenfinvpda");
    cout << rlcoder.getKof() << "\n";

    LZCoder lzcoder;
    cout << "\n\n---------\n\nМетод сжатия Лемпеля-Зива 1977 года (LZ77):\n";
  /*  cout << lzcoder.LZ77("abracadabracadaabracad", 10);
    cout << lzcoder.getKof() << "\n";
    cout << lzcoder.LZ77("bacccaaaaaaaaa", 7);
    cout << lzcoder.getKof() << "\n";*/
    cout << lzcoder.LZ77("10101001101100111010", 15);
    cout << lzcoder.getKof() << "\n";

    cout << "\n\n---------\n\nМетод сжатия Лемпеля-Зива 1978 года (LZ78):\n";
   /* cout << lzcoder.LZ78("abracadabracadaabracad");
    cout << rlcoder.getKof() << "\n";
    cout << lzcoder.LZ78("bacccaaaaaaaaa");
    cout << rlcoder.getKof() << "\n";*/
    cout << lzcoder.LZ78("bigbonebigborebigbo");
    cout << rlcoder.getKof() << "\n";


    ShennonFannoCoder sfcoder;
    cout << "\n\n---------\n\nМетод сжатия Шеннона-Фано:\n";
    sfcoder.PrintDecodeString(sfcoder.EncodeString("Кот пошел за молоком, а котята кувырком. Кот пришел без молока, а котята ха-ха-ха."));


    HuffmanCoder hcoder;
    cout << "\n\n---------\n\nМетод сжатия Хаффмана:\n";
    hcoder.PrintDecodeString(hcoder.EncodeString("Яковлева Елизавета ИКБО-10-20"));
    cout << "\n\n---------\n\nМетод сжатия Хаффмана:\n";
    hcoder.PrintDecodeString(hcoder.EncodeString("Составить программу сжатия текста, используя метод RLE (run length encoding/кодирование длин серий/групповое кодирование). Рассчитать коэффициент сжатия в тестах. Провести тестирование на длинной серии повторяющихся символов."));
}


/*
    cout << LZ77("bacbacdrbacdr", 5) << "\n";
    cout << LZ77("bacbacdbacdr", 4) << "\n";
    cout << LZ77("10101001101100111010", 7) << "\n";

    cout << LZ78("abacababacabc") << "\n";
    cout << LZ78("abracadabraabrababra") << "\n";
    cout << LZ78("bigbonebigborebigbo") << "\n";
*/

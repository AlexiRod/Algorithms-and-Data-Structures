#include <iostream>
#include <string>
#include <utility>

using std::string;

class BloomFilter {
    size_t bits_, hash_functions_;
    int64_t size_ = 0;
    int64_t capacity_ = 10;
    int64_t requests_counter_ = 0;
    int64_t false_responses_counter_ = 0;
    string* all_strings_ = new string[capacity_];
    int* arr_;

    void insert(string str) {
        if (size_ == capacity_) {
            capacity_ *= 2;

            string* new_all_strings = new std::string[capacity_];
            for (int64_t i = 0; i < size_; ++i) {
                new_all_strings[i] = all_strings_[i];
            }

            std::swap(all_strings_, new_all_strings);
            delete[] new_all_strings;
        }
        all_strings_[size_] = str;
        ++size_;
    }

    bool isContained(std::string str) {
        for (int64_t i = 0; i < size_; ++i) {
            if (all_strings_[i] == str) {
                return true;
            }
        }
        return false;
    }

public:
    BloomFilter(size_t hashes, size_t bits) {
        arr_ = new int[bits];
        hash_functions_ = hashes;
        bits_ = bits;

        for (size_t i = 0; i < bits; ++i) {
            arr_[i] = 0;
        }
    }

    ~BloomFilter() {
        delete[] arr_;
        delete[] all_strings_;
    }

    void add(const string& obj) {
        int64_t ind = std::hash<string>{}(obj) % bits_;

        arr_[(ind + bits_ - 1) % bits_] = 1;
        for (size_t i = 1; i < hash_functions_; ++i) {
            ind = std::hash<std::string>{}(obj + std::to_string(i)) % bits_;
            arr_[(ind + bits_ - 1) % bits_] = 1;
        }
        insert(obj);
    }

    bool verify(const std::string& obj) {
        ++requests_counter_;
        int64_t ind = std::hash<std::string>{}(obj) % bits_;

        if (arr_[(ind + bits_ - 1) % bits_] == 0) {
            return false;
        }

        for (size_t i = 1; i < hash_functions_; ++i) {
            ind = std::hash<std::string>{}(obj + std::to_string(i)) % bits_;
            if (arr_[(ind + bits_ - 1) % bits_] == 0) {
                return false;
            }
        }

        if (!isContained(obj)) {
            ++false_responses_counter_;
        }
        return true;
    }

    size_t numberOfHashFunctions() const {
        return hash_functions_;
    }

    size_t numberOfBits() const {
        return bits_;
    }

    double getFPRate() const {
        if (requests_counter_ == 0) {
            return 0;
        }
        return static_cast<double>(false_responses_counter_) /
               static_cast<double>(requests_counter_);
    }
};

#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <fstream>

/*
 * ПиАА 2022, Родионов Алексей Алексеевич, БПИ208
 * Родионов Алексей Алексеевич, БПИ208
 * Среда разработки: Visual Studio 2022
 * Сделано:
 * Реализация всех сортировок
 * Указатели на функции и константные ссылки
 * Графики и отчет по заданию
 * Соблюдение правил выполнения работы (слайды 46-47)
 * Применение ООП подхода
 * Комментарии к методам
  */


// Класс, хранящий в себе все 12 сортировок.
class Sorter {
private:

    // Вспомогательный метод правого бинарного поиска для сортировки бинарными вставками.
    static int rightBinSearch(std::vector<int>* a, const int& n, const int& elem) {
        int left = 0;
        int right = n;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (elem <= a->at(mid)) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }
        if (a->at(left) == elem) {
            while (left < n && a->at(left + 1) == elem) {
                left++;
            }
            ++left;
        }
        return left;
    }

    // Вспомогательный метод слияния для сортировки слиянием.
    static void merge(std::vector<int>* arr, const int& left, const int& right, const int& mid) {
        int f_ind = 0;
        int s_ind = 0;
        int* res = new int[right - left];

        while (left + f_ind < mid && mid + s_ind < right) {
            if (arr->at(left + f_ind) < arr->at(mid + s_ind)) {
                res[f_ind + s_ind] = arr->at(left + f_ind);
                f_ind++;
            }
            else {
                res[f_ind + s_ind] = arr->at(mid + s_ind);
                s_ind++;
            }
        }

        while (left + f_ind < mid) {
            res[f_ind + s_ind] = arr->at(left + f_ind);
            f_ind++;
        }
        while (mid + s_ind < right) {
            res[f_ind + s_ind] = arr->at(mid + s_ind);
            s_ind++;
        }

        for (int i = 0; i < f_ind + s_ind; ++i) {
            arr->at(left + i) = res[i];
        }
        delete[] res;
    }

    // Вспомогательный метод быстрой сортировки с разбиением Хоара
    static void qsortHoare(std::vector<int>* arr, const int& first, const int& last) {
        int left = first, right = last, pivot = arr->at((first + last) / 2);
        do {
            while (arr->at(left) < pivot) {
                left++;
            }
            while (arr->at(right) > pivot) {
                right--;
            }

            if (left <= right) {
                if (arr->at(left) > arr->at(right)) {
                    std::swap(arr->at(left), arr->at(right));
                }
                left++;
                right--;
            }
        } while (left <= right);

        if (left < last) {
            qsortHoare(arr, left, last);
        }
        if (first < right) {
            qsortHoare(arr, first, right);
        }
    }

    // Вспомогательный метод нахождения указателя на опорный элемент для быстрой сортировки с разбиением Ломуто.
    static int* findPivot(int* first, int* last) {
        if (last - first < 2) {
            return first;
        }
        if (*first > *(--last)) {
            std::swap(*first, *last);
        }

        auto pivot_pos = first;
        auto pivot = *first;
        do {
            ++first;
        } while (*first < pivot);

        for (auto ind = first + 1; ind < last; ++ind) {
            auto val = *ind;
            auto less = -int(val < pivot);
            auto d = less & (ind - first);
            
            first[d] = *first;
            first -= less;
            ind[-d] = val;
        }

        --first;
        *pivot_pos = *first;
        *first = pivot;
        return first;
    }

    // Вспомогательный метод для быстрой сортировки с разбиением Ломуто.
    static void qsortLomuto(std::vector<int>* arr, int* first, int* last) {
        if (first < last) {
            int* pivot = findPivot(first, last);
            qsortLomuto(arr, first, pivot);
            qsortLomuto(arr, pivot + 1, last);
        }
    }

    // Вспомогательный класс бинарной кучи для сортировки кучей.
    class BinaryHeap {
    private:
        std::vector<int> arr_;

        // Вспомогательный метод "балансировки" кучи.
        void heapify(int ind) {
            bool flag = true;
            while (flag) {
                int max = ind;
                int left = 2 * ind + 1, right = 2 * ind + 2;
                if (left < arr_.size() && arr_[left] > arr_[max]) {
                    max = left;
                }
                if (right < arr_.size() && arr_[right] > arr_[max]) {
                    max = right;
                }
                if (max == ind) {
                    flag = false;
                    break;
                }
                std::swap(arr_[ind], arr_[max]);
                ind = max;
            }
        }

        // Вспомогательный метод построения кучи.
        void buildHeap(std::vector<int> vec) {
            arr_ = vec;
            for (int i = vec.size() / 2; i >= 0; --i) {
                heapify(i);
            }
        }

        // Вспомогательный метод извлечения корневого максимума.
        int popMax() {
            int max = arr_[0];
            arr_[0] = arr_[arr_.size() - 1];
            arr_.pop_back();
            return max;
        }

    public:
        // Вспомогательный метод сортировки кучей.
        std::vector<int>* heapSort(std::vector<int>* vec) {
            buildHeap(*vec);
            for (int i = vec->size() - 1; i >= 0; --i) {
                vec->at(i) = popMax();
                heapify(0);
            }
            return vec;
        }
    };

public:
    // Указатели на функции сортировок.
    void (*sorts[12])(std::vector<int>*, const int&) = {
            sortSelection,
            sortBubble,
            sortBubbleFirst,
            sortBubbleFirstAndSecond,
            sortInsertion,
            sortInsertionBinary,
            sortCounting,
            sortRadix,
            sortMerge,
            sortQuickSortHoare,
            sortQuickSortLomuto,
            sortHeap
    };

    // Реализация сортировки выбором.
    static void sortSelection(std::vector<int>* arr, const int& n) {
        for (int i = 0; i < n - 1; ++i) {
            int min = i;
            for (int j = i + 1; j < n; ++j) {
                if (arr->at(j) < arr->at(min)) {
                    min = j;
                }
            }
            if (i != min) {
                std::swap(arr->at(i), arr->at(min));
            }
        }
    }

    // Реализация сортировки пузырьком.
    static void sortBubble(std::vector<int>* arr, const int& n) {
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (arr->at(j) > arr->at(j + 1)) {
                    std::swap(arr->at(j), arr->at(j + 1));
                }
            }
        }
    }

    // Реализация сортировки пузырьком с первым условием Айверсона.
    static void sortBubbleFirst(std::vector<int>* arr, const int& n) {
        int bound = 0;
        bool flag = true;
        while (flag) {
            flag = false;
            for (int i = 0; i < n - bound - 1; ++i) {
                if (arr->at(i) > arr->at(i + 1)) {
                    std::swap(arr->at(i), arr->at(i + 1));
                    flag = true;
                }
            }
            ++bound;
        }
    }
 
    // Реализация сортировки пузырьком с первым и вторым условием Айверсона.
    static void sortBubbleFirstAndSecond(std::vector<int>* arr, const int& n) {
        int bound = n - 2;
        bool flag = true;
        while (flag) {
            flag = false;
            int new_bound = 0;
            for (int j = 0; j <= bound; ++j) {
                if (arr->at(j) > arr->at(j + 1)) {
                    std::swap(arr->at(j), arr->at(j + 1));
                    flag = true;
                    new_bound = j;
                }
            }
            bound = new_bound;
        }
    }

    // Реализация сортировки вставками.
    static void sortInsertion(std::vector<int>* arr, const int& n) {
        for (int i = 1; i < n; ++i) {
            int j = i - 1;
            if (arr->at(i) >= arr->at(i - 1)) {
                continue;
            }
            while (j >= 0 && arr->at(j) > arr->at(j + 1)) {
                std::swap(arr->at(j), arr->at(j + 1));
                --j;
            }
        }
    }

    // Реализация сортировки бинарными вставками.
    static void sortInsertionBinary(std::vector<int>* arr, const int& n) {
        for (int i = 1; i < n; ++i) {
            int j = i - 1;
            if (arr->at(i) >= arr->at(i - 1)) {
                continue;
            }
            int k = rightBinSearch(arr, i, arr->at(i));
            for (int m = j; m >= k; --m) {
                std::swap(arr->at(m), arr->at(m + 1));
            }
        }
    }

    // Реализация сортировки подсчетом.
    static void sortCounting(std::vector<int>* arr, const int& n) {
        int max = INT32_MIN;
        int min = 0; // Для случая сдвига
        for (int i = 0; i < n; ++i) {
            max = arr->at(i) > max ? arr->at(i) : max;
        }
        int k = max - min + 1;
        std::vector<int> counts(k);
        std::vector<int> res(n);
        for (int elem : *arr) {
            counts[elem - min]++;
        }
        for (int i = 1; i < k; ++i) {
            counts[i] += counts[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            counts[arr->at(i) - min]--;
            res[counts[arr->at(i) - min]] = arr->at(i);
        }
        *arr = res;
    }

    // Реализация цифровой сортировки.
    static void sortRadix(std::vector<int>* arr, const int& n) {
        int deg = 1, max = 0;
        std::vector<int> res(n);
        for (int i = 0; i < n; ++i) {
            max = arr->at(i) > max ? arr->at(i) : max;
        }
        while (max / deg > 0) {
            std::vector<int> count(256, 0);
            for (int i = 0; i < n; ++i) {
                ++count[(arr->at(i) / deg) % 128 + 127];
            }
            for (int i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }
            for (int i = n - 1; i >= 0; --i) {
                res[--count[(arr->at(i) / deg) % 128 + 127]] = arr->at(i);
            }
            for (int i = 0; i < n; ++i) {
                arr->at(i) = res[i];
            }
            deg *= 128;
        }
    }

    // Реализация сортировки слиянием.
    static void sortMerge(std::vector<int>* arr, const int& n) {
        for (int i = 1; i < n; i *= 2) {
            for (int j = 0; j < n - i; j += 2 * i) {
                merge(arr, j, std::min(j + 2 * i, n), j + i);
            }
        }
    }

    // Реализация быстрой сортировки с разбиением Хоара.
    static void sortQuickSortHoare(std::vector<int>* arr, const int& n) {
        qsortHoare(arr, 0, n - 1);
    }

    // Реализация быстрой сортировки с разбиением Ломуто.
    static void sortQuickSortLomuto(std::vector<int>* arr, const int& n) {
        qsortLomuto(arr, &arr->front(), &arr->back() + 1);
    }
    
    // Реализация сортировки кучей.
    static void sortHeap(std::vector<int>* arr, const int& n) {
        BinaryHeap heap;
        *arr = *heap.heapSort(arr);
    }
};


// Класс для замеров времени, записи в файл и тестом сортировок.
class SorterTester {
private:
    // Константы для задания диапозонов генерации и размеров массивов.
    const int kMaxSize = 4100;
    const int kSmallBegin = 50;
    const int kSmallEnd = 300;
    const int kSmallStep = 10;
    const int kBigBegin = 100;
    const int kBigEnd = 4100;
    const int kBigStep = 100;

    // Поток для записи в CSV-файл.
    std::ofstream table;

    // Тип заполнения массива.
    enum ArrayFillingType {
        SMALL_RANDOM,
        BIG_RANDOM,
        ALMOST_SORTED,
        REVERSE_SORTED
    };

    // Вспомогательный метод проверки на отсортированность.
    bool isSorted(std::vector<int>* arr) {
        int n = arr->size();
        for (int i = 1; i < n; ++i) {
            if (arr->at(i - 1) > arr->at(i)) {
                return false;
            }
        }
        return true;
    }

    // Вспомогательный метод заполнения массива случайными числами из диапозона [min, mix]
    void fillArray(std::vector<int>* arr, const int& n, const int& min, const int& max) {
        // Генератор случйных чисел.
        std::mt19937 gen(time(nullptr));
        std::uniform_int_distribution<int> dist(0, max);

        arr->resize(n);
        for (int i = 0; i < n; ++i) {
            arr->at(i) = dist(gen);
        }
    }

    // Вспомогательный метод перемешивания заданного количества элементов массива.
    void shuffleArray(std::vector<int>* arr, const int& swaps) {
        // Генерация случайных индексов для перемещения.
        std::mt19937 gen(time(nullptr));
        std::uniform_int_distribution<int> dist(0, static_cast<int>(arr->size()) - 1);

        // Перемещения случайных элементов заданного количества раз.
        for (int i = 0; i < swaps; ++i) {
            std::swap(arr->at(dist(gen)), arr->at(dist(gen)));
        }
    }

    // Вспомогательный метод для записи в файл, необходмый для защиты.
    void writeArray(const std::vector<int>* arr, const std::string& path) {
        std::ofstream file(path);
        for (int elem : *arr) {
            file << elem << ' ';
        }
        file << '\n';
        file.close();
    }
    
    // Вспомогательный метод генерации массива в заисимости от переданного типа.
    void generateArray(std::vector<int>* arr, const int& n, const ArrayFillingType& type) {
        switch (type) {
        case SMALL_RANDOM: // Заполнение малыми числами
            fillArray(arr, n, 0, 5);
            break;
        case BIG_RANDOM: // Заполнение большими числами
            fillArray(arr, n, 0, 4000);
            break;
        case ALMOST_SORTED: // Практически отсортированный массив, в котором десятая часть элементов перемешана
            fillArray(arr, n, 0, 4000);
            Sorter::sortHeap(arr, n);
            shuffleArray(arr, n / 10);
            break;
        case REVERSE_SORTED: // Обратно отсортированный массив.
            arr->clear();
            for (int i = n; i >= 0; --i) {
                arr->push_back(i);
            }
        }
    }

    // Вспомогательный метод для сортировки массива заданным образом и замером времени работы.
    size_t sortArray(std::vector<int>* arr, const int& n, void(sort) (std::vector<int>*, const int&)) {
        // Копирование массива.
        std::vector<int> duplicate;
        for (int i = 0; i < n; ++i) {
            duplicate.push_back(arr->at(i));
        }

        // Создание часов для замера времени до и после сортировки.
        auto start = std::chrono::high_resolution_clock::now();
        sort(&duplicate, n);
        auto end = std::chrono::high_resolution_clock::now();

        // Проверка на отсортированность.
        if (!isSorted(&duplicate)) {
            std::cout << "Ups, sort doesn't work...\n";
        }
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    // Вспомогательный метод заполнения таблицы временем работы сортировки.
    void fillTable(std::vector<int> arr, const int& begin, const int& end, const int& step, void(sort)(std::vector<int>*, const int&)) {
        for (int size = begin; size <= end; size += step) {
            // Первые два прогона пропущены и не учитываются.
            sortArray(&arr, size, sort);
            sortArray(&arr, size, sort);
            // Сумма всех времен сортировки.
            size_t sum = 0;
            for (int i = 0; i < 5; ++i) {
                sum += sortArray(&arr, size, sort);
            }
            // Подсчет среднего значения из 5 тестов.
            double average = sum / 5.0;
            table << std::fixed << std::setprecision(5) << average << ";";
        }
    }

public:
    // Основной метод тестирования сортировок.
    void testSorts() {
        table.open("table.csv");
        Sorter sorter;
        // Все типы заполнения массивов.
        ArrayFillingType filling_types[] = { SMALL_RANDOM, BIG_RANDOM, ALMOST_SORTED, REVERSE_SORTED };

        for (auto sort_type : sorter.sorts) { // Итерация по сортировкам класса Sorter.
            for (auto filling_type : filling_types) { // Итерация по способам заполнения массива.
                std::vector<int> arr;
                generateArray(&arr, kMaxSize, filling_type);
                // Замер времени и запись в таблицу сначала для малых чисел, затем для больших. Границы регулируются через константы.
                fillTable(arr, kSmallBegin, kSmallEnd, kSmallStep, sort_type);
                fillTable(arr, kBigBegin, kBigEnd, kBigStep, sort_type);
                table << "\n";
            }
            std::cout << "sort done\n";
        }
        table.close();
    }
};

// Основной метод программы.
int main() {
    SorterTester tester;
    std::cout << "tester starts work\n";
    tester.testSorts();
    std::cout << "tester ends work\n";
}

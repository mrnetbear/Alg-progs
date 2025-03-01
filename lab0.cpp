#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

template<typename T>
T calcsum(T value){
    size_t pos = 0;
    int sum = 0;
    while (value > 0){
        if (pos % 2) {
            sum += value % 10;
        }
        value /= 10;
        pos++;
    }
    return value = sum;
} 

template<typename T>
T calcN(T value){
    if (!value) return 0;
    return (log10(value) + 1) / 2;
}

template<typename T>
void QuickSort(int first, int last, std::vector<T> &A){
    int pivot;
    int l = first,
        r = last;
    pivot = calcN(A[(l + r) / 2]);
    while (l <= r){
        while (calcN(A[l]) < pivot) l++;
        while (calcN(A[r]) > pivot) r--;
        if (l <= r){
            std::swap(A[l], A[r]);
            l++;
            r--;
        }
    }
    if (first < r) QuickSort(first, r, A);
    if (l < last) QuickSort(l, last, A);
}

template<typename T>
void print_vector(const std::vector<T>& vec){
    for(const auto& value : vec){
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
int main() {
    size_t n;
    std::vector<unsigned int> init_vlaues; 
    std::cin >> n;
    if (std::cin.fail() || std::cin.eof() || n < 0 || (int)n < 0)
        return -1;
    for (size_t i = 0; i < n; ++i){
        unsigned int value;
        std::cin >> value;
        if (std::cin.fail() || std::cin.eof() || n < 0 || (int)n < 0)
        return -1;
        init_vlaues.push_back(value);
    }

    std::cout << "----------------Initial----------------" << std::endl;
    print_vector(init_vlaues);
    std::cout << "---------------Sorted-----------------" << std::endl;

    QuickSort(0, n-1, init_vlaues);

    print_vector(init_vlaues);
    std::cout << "----------------SortedSums----------------" << std::endl;

    for (auto &a : init_vlaues){
        a = calcsum(a);
    }

    print_vector(init_vlaues);
    std::cout << "--------------------------------" << std::endl;

    
    return 0;
}
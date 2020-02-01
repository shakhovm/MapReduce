#include <iostream>
#include <algorithm>
#include "my_reduce.h"
#include "my_map.h"


#define THREAD_NUMBER 5
#define ELEMENT_NUMBER 100000000

template <typename T, class BinaryOp, class InputIt>
T my_simpleReduce(InputIt first, InputIt last, BinaryOp operation, T value) {
    while (first != last) {
        value = operation(value, *first++);
    }
    return value;
}

template <class UnaryOp, class InputIt>
void my_simpleMap(InputIt first, InputIt last, UnaryOp operation) {
    while (first != last)
        *first++ = operation(*first);
}

void reduce_test(size_t values_number, size_t thread_number)
{
    std::vector<int> v(values_number, 2);

    auto t1 = std::chrono::high_resolution_clock::now();
    int x = my_reduce(v.begin(), v.end(), [](int x, int y){return x + y;}, thread_number, 1);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "for parallel my_reduce: " << ms.count() << std::endl;
    std::cout << "Sum is: " << x << std::endl;



    t1 = std::chrono::high_resolution_clock::now();
    int k = my_simpleReduce(v.begin(), v.end(), [](int x, int y) { return x + y; }, 1);

    t2 = std::chrono::high_resolution_clock::now();
    ms = t2 - t1;
    std::cout << "for simple my_reduce: " << ms.count() << std::endl;
    std::cout << "Sum is: " << k << std::endl;
}

int newF(int x) {
    return x*2;
}
void map_test(size_t values_number, size_t thread_number)
{
    std::vector<int> v1(values_number, 1);
    std::vector<int> v2(v1);
    auto t1 = std::chrono::high_resolution_clock::now();
    my_map(v1.begin(), v1.end(), [](int x){return x*2;}, thread_number);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = t2 - t1;
    std::cout << "for parallel my_map: " << ms.count() << std::endl;



    t1 = std::chrono::high_resolution_clock::now();
    my_simpleMap(v2.begin(), v2.end(), [](int x){return x*2;});

    t2 = std::chrono::high_resolution_clock::now();
    ms = t2 - t1;
    std::cout << "for simple my_map: " << ms.count() << std::endl;

}

int main() {
    reduce_test(ELEMENT_NUMBER, THREAD_NUMBER);
    map_test(ELEMENT_NUMBER, THREAD_NUMBER);
    //std::cout << "Your number of cores: " << std::thread::hardware_concurrency() << std::endl;
    return 0;
}
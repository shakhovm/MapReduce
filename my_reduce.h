//
// Created by user on 01.02.2020.
//

#ifndef MAPREDUCE_MY_REDUCE_H
#define MAPREDUCE_MY_REDUCE_H

#include <thread>
#include <mutex>
#include <vector>

template <typename T, class BinaryOp, class InputIt>
static void simple_reduce(InputIt first, InputIt last, BinaryOp operation,
                          T value, std::vector<T>& values,
                          std::mutex& mt);

template <typename T, class BinaryOp, class InputIt>
T my_reduce(InputIt first, InputIt last,
            BinaryOp operation, size_t thread_number,
            T init);

template <typename T, class BinaryOp, class InputIt>
void simple_reduce(InputIt first, InputIt last, BinaryOp operation, T value, std::vector<T>& values,
                   std::mutex& mt) {

    while (first != last) {
        value = operation(value, *first++);

    }
    mt.lock();
    values.push_back(value);
    mt.unlock();
}


template <typename T, class BinaryOp, class InputIt>
T my_reduce(InputIt first, InputIt last, BinaryOp operation, size_t thread_number, T init) {
    std::thread thread_vector[thread_number];
    std::vector<T> values(thread_number);
    size_t size = std::distance(first, last);

    std::mutex mt;

    for (size_t i = 0; i < thread_number; ++i) {

        thread_vector[i] = std::thread(simple_reduce<T, BinaryOp, InputIt>,
                first + (i*size)/thread_number,
                first + std::min((i+1)*size/thread_number, size),
                operation, *(first + (i*size)/thread_number),
                std::ref(values), std::ref(mt));

    }

    for (size_t i = 0; i < thread_number; ++i) {
        thread_vector[i].join();
    }

    for(auto& x: values) {
        init = operation(init, x);
    }

    return init;
}


#endif //MAPREDUCE_MY_REDUCE_H

//
// Created by user on 01.02.2020.
//

#ifndef MAPREDUCE_MY_MAP_H
#define MAPREDUCE_MY_MAP_H



#include <thread>
#include <mutex>
#include <vector>

template <class UnaryOp, class InputIt>
static void simple_map(InputIt first, InputIt last, UnaryOp operation);

template <class UnaryOp, class InputIt>
void my_map(InputIt first, InputIt last,
            UnaryOp operation, size_t thread_number);

template <class UnaryOp, class InputIt>
void simple_map(InputIt first, InputIt last, UnaryOp operation) {

    while (first != last) {
        *first++ = operation(*first);
    }
}


template <class UnaryOp, class InputIt>
void my_map(InputIt first, InputIt last, UnaryOp operation, size_t thread_number) {

    std::thread thread_vector[thread_number];
    size_t size = std::distance(first, last);

    for (size_t i = 0; i < thread_number; ++i) {

        thread_vector[i] = std::thread(simple_map<UnaryOp, InputIt>,
                first + (i*size)/thread_number,
                first + std::min((i+1)*size/thread_number, size),
                operation);

    }

    for (size_t i = 0; i < thread_number; ++i) {
        thread_vector[i].join();
    }
}

#endif //MAPREDUCE_MY_MAP_H

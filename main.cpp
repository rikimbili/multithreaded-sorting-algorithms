#include <iostream>
#include <chrono>
#include <random>
#include "sorting_algorithms.hpp"
#include "table.h"
#include "spinners.hpp"

using namespace std;
using namespace spinners;

const int ARR_SIZE = 100000;

VariadicTable<string, string, string, string, string> vt(
        {"Sorting Algorithm", "non-concurrent (time)", "2 threads (time)", "4 threads (time)", "8 threads (time)"}, 24);
Spinner *spinner = new Spinner();

int *getArrCopy(const int arr[]) {
    int *arr_copy = new int[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++) {
        arr_copy[i] = arr[i];
    }
    return arr_copy;
}

long getBubbleSortTime(int arr[], int num_threads) {
    auto arr_copy = getArrCopy(arr);
    auto t1 = chrono::high_resolution_clock::now();
    if (num_threads == 1) { bubbleSort(arr_copy, 0, ARR_SIZE - 1); }
    else { concurrentBubbleSort(arr_copy, ARR_SIZE, num_threads); }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();

}

void benchmarkBubbleSort(int arr[]) {
    spinner->setText("Benchmarking bubble sort...");
    spinner->start();

    auto non_concurrent_time = getBubbleSortTime(arr, 1);
    auto concurrent_time_2 = getBubbleSortTime(arr, 2);
    auto concurrent_time_4 = getBubbleSortTime(arr, 4);
    auto concurrent_time_8 = getBubbleSortTime(arr, 8);

    vt.addRow("Bubble Sort", to_string(non_concurrent_time) + "ms",
              to_string(concurrent_time_2) + "ms",
              to_string(concurrent_time_4) + "ms",
              to_string(concurrent_time_8) + "ms");

    spinner->stop();
}

int main() {
    cout << "\nBenchmarking classic sorting algorithms and their concurrent implementations \n\n";
    spinner->setInterval(100);
    spinner->setSymbols("dots");

    // Initialize heap array of size ARR_SIZE with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100000);
    int *arr = new int[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = dis(gen);
    }


    benchmarkBubbleSort(arr);

    vt.print(cout);

    return 0;
}

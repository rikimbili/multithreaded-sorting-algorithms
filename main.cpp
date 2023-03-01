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
        {"Sorting Algorithm", "Time (non-concurrent)", "Time (2 threads)", "Time (4 threads)", "Time (8 threads)"}, 24);
Spinner *spinner = new Spinner();

int *getArrCopy(const int arr[]) {
    int *arr_copy = new int[ARR_SIZE];
    for (int i = 0; i < ARR_SIZE; i++) {
        arr_copy[i] = arr[i];
    }
    return arr_copy;
}

// Mainly for debugging purposes
void isSorted(int arr[], string arrName = "Array") {
    bool isSorted = true;
    for (int i = 0; i < ARR_SIZE - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            isSorted = false;
            cout << arrName << " is not sorted" << endl;
            cout << "arr[" << i << "] = " << arr[i] << " and arr[" << i + 1 << "] = " << arr[i + 1] << endl << endl;
            // return;
        }
    }
    if (isSorted) cout << arrName << " is sorted" << endl << endl;
}

long getBubbleSortTime(int arr[], int numThreads) {
    auto arr_copy = getArrCopy(arr);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        bubbleSort(arr_copy, 0, ARR_SIZE);
        isSorted(arr_copy, "Array with 1 thread");
    }
    else { 
        concurrentBubbleSort(arr_copy, ARR_SIZE, numThreads);
        isSorted(arr_copy, "Array with " + to_string(numThreads) + " threads");
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
}

void benchmarkBubbleSort(int arr[]) {
    spinner->setText("Benchmarking bubble sort...");
    spinner->start();

    cout << "Benchmarking bubble sort..." << endl;
    auto non_concurrent_time = getBubbleSortTime(arr, 1);
    auto concurrent_time_2 = getBubbleSortTime(arr, 2);
    auto concurrent_time_4 = getBubbleSortTime(arr, 4);
    auto concurrent_time_8 = getBubbleSortTime(arr, 8);
    
    vt.addRow("Bubble Sort", to_string(non_concurrent_time) + "ms",
              to_string(concurrent_time_2) + "ms (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "ms (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "ms (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

    spinner->stop();
}

long getMergeSortTime(int arr[], int numThreads) {
    auto arr_copy = getArrCopy(arr);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        mergeSort(arr_copy, 0, ARR_SIZE - 1); 
        isSorted(arr_copy, "Array with 1 thread");
    }
    else { 
        concurrentMergeSort(arr_copy, 0, ARR_SIZE - 1, numThreads);
        isSorted(arr_copy, "Array with " + to_string(numThreads) + " threads"); 
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
}

void benchmarkMergeSort(int arr[]) {
    spinner->setText("Benchmarking merge sort...");
    spinner->start();

    cout << "Benchmarking merge sort..." << endl;
    auto non_concurrent_time = getMergeSortTime(arr, 1);
    auto concurrent_time_2 = getMergeSortTime(arr, 2);
    auto concurrent_time_4 = getMergeSortTime(arr, 4);
    auto concurrent_time_8 = getMergeSortTime(arr, 8);
    
    vt.addRow("Merge Sort", to_string(non_concurrent_time) + "ms",
              to_string(concurrent_time_2) + "ms (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "ms (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "ms (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

    spinner->stop();
}

long getRadixSortTime(int arr[], int numThreads) {
    auto arr_copy = getArrCopy(arr);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        radixSort(arr_copy, 0, ARR_SIZE);
        isSorted(arr_copy, "Array with 1 thread");
    }
    else { 
        concurrentRadixSort(arr_copy, ARR_SIZE, numThreads);
        isSorted(arr_copy, "Array with " + to_string(numThreads) + " threads");
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
}

void benchmarkRadixSort(int arr[]) {
    spinner->setText("Benchmarking radix sort...");
    spinner->start();

    cout << "Benchmarking radix sort..." << endl;
    auto non_concurrent_time = getRadixSortTime(arr, 1);
    auto concurrent_time_2 = getRadixSortTime(arr, 2);
    auto concurrent_time_4 = getRadixSortTime(arr, 4);
    auto concurrent_time_8 = getRadixSortTime(arr, 8);
    
    vt.addRow("Radix Sort", to_string(non_concurrent_time) + "ms",
              to_string(concurrent_time_2) + "ms (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "ms (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "ms (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

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
    benchmarkMergeSort(arr);
    benchmarkRadixSort(arr);

    vt.print(cout);

    return 0;
}

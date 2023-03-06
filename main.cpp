#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "sorting_algorithms.hpp"
#include "table.h"
#include "spinners.hpp"

using namespace std;
using namespace spinners;

ofstream sortingData;

VariadicTable<string, string, string, string, string, string> vt(
        {"Sorting Algorithm", "Array Size", "Time (non-concurrent)", "Time (2 threads)", "Time (4 threads)", "Time (8 threads)"}, 24);
Spinner *spinner = new Spinner();

int *getArrCopy(const int arr[], int arrSize) {
    int *arr_copy = new int[arrSize];
    for (int i = 0; i < arrSize; i++) {
        arr_copy[i] = arr[i];
    }
    return arr_copy;
}

// Mainly for debugging purposes
void isSorted(int arr[], int arrSize, string arrName = "Array") {
    bool isSorted = true;
    for (int i = 0; i < arrSize - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            isSorted = false;
            cout << arrName << " is not sorted" << endl;
            cout << "arr[" << i << "] = " << arr[i] << " and arr[" << i + 1 << "] = " << arr[i + 1] << endl << endl;
            // return;
        }
    }
    if (isSorted) cout << arrName << " is sorted" << endl << endl;
}

long getBubbleSortTime(int arr[], int arrSize, int numThreads) {
    auto arr_copy = getArrCopy(arr, arrSize);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        bubbleSort(arr_copy, 0, arrSize);
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentBubbleSort(arr_copy, arrSize, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads");
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}

void benchmarkBubbleSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking bubble sort...");
    spinner->start();

    cout << "Benchmarking bubble sort..." << endl;

    auto non_concurrent_time = getBubbleSortTime(arr, arrSize, 1);
    sortingData << "Bubble Sort," << arrSize << "," << non_concurrent_time << "," << "1" << endl;
    auto concurrent_time_2 = getBubbleSortTime(arr, arrSize, 2);
    sortingData << "Bubble Sort," << arrSize << "," << concurrent_time_2 << "," << "2" << endl;
    auto concurrent_time_4 = getBubbleSortTime(arr, arrSize, 4);
    sortingData << "Bubble Sort," << arrSize << "," << concurrent_time_4 << "," << "4" << endl;
    auto concurrent_time_8 = getBubbleSortTime(arr, arrSize, 8);
    sortingData << "Bubble Sort," << arrSize << "," << concurrent_time_8 << "," << "8" << endl;
    
    vt.addRow("Bubble Sort", to_string(arrSize), to_string(non_concurrent_time) + "μs",
              to_string(concurrent_time_2) + "μs (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "μs (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "μs (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

    spinner->stop();
}

long getMergeSortTime(int arr[], int arrSize, int numThreads) {
    auto arr_copy = getArrCopy(arr, arrSize);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        mergeSort(arr_copy, 0, arrSize - 1); 
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentMergeSort(arr_copy, 0, arrSize - 1, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads"); 
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}

void benchmarkMergeSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking merge sort...");
    spinner->start();

    cout << "Benchmarking merge sort..." << endl;
    auto non_concurrent_time = getMergeSortTime(arr, arrSize, 1);
    sortingData << "Merge Sort," << arrSize << "," << non_concurrent_time << "," << "1" << endl;
    auto concurrent_time_2 = getMergeSortTime(arr, arrSize, 2);
    sortingData << "Merge Sort," << arrSize << "," << concurrent_time_2 << "," << "2" << endl;
    auto concurrent_time_4 = getMergeSortTime(arr, arrSize, 4);
    sortingData << "Merge Sort," << arrSize << "," << concurrent_time_4 << "," << "4" << endl;
    auto concurrent_time_8 = getMergeSortTime(arr, arrSize, 8);
    sortingData << "Merge Sort," << arrSize << "," << concurrent_time_8 << "," << "8" << endl;
    
    vt.addRow("Merge Sort", to_string(arrSize), to_string(non_concurrent_time) + "μs",
              to_string(concurrent_time_2) + "μs (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "μs (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "μs (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

    spinner->stop();
}

long getRadixSortTime(int arr[], int arrSize, int numThreads) {
    auto arr_copy = getArrCopy(arr, arrSize);
    // isSorted(arr_copy, "Original array");
    auto t1 = chrono::high_resolution_clock::now();
    if (numThreads == 1) { 
        radixSort(arr_copy, 0, arrSize);
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentRadixSort(arr_copy, arrSize, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads");
    }
    delete arr_copy;
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}

void benchmarkRadixSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking radix sort...");
    spinner->start();

    cout << "Benchmarking radix sort..." << endl;
    auto non_concurrent_time = getRadixSortTime(arr, arrSize, 1);
    sortingData << "Radix Sort," << arrSize << "," << non_concurrent_time << "," << "1" << endl;
    auto concurrent_time_2 = getRadixSortTime(arr, arrSize, 2);
    sortingData << "Radix Sort," << arrSize << "," << concurrent_time_2 << "," << "2" << endl;
    auto concurrent_time_4 = getRadixSortTime(arr, arrSize, 4);
    sortingData << "Radix Sort," << arrSize << "," << concurrent_time_4 << "," << "4" << endl;
    auto concurrent_time_8 = getRadixSortTime(arr, arrSize, 8);
    sortingData << "Radix Sort," << arrSize << "," << concurrent_time_8 << "," << "8" << endl;
    
    vt.addRow("Radix Sort", to_string(arrSize), to_string(non_concurrent_time) + "μs",
              to_string(concurrent_time_2) + "μs (" +
              to_string((concurrent_time_2 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_4) + "μs (" +
              to_string((concurrent_time_4 - non_concurrent_time) * 100 / non_concurrent_time) + "%)",
              to_string(concurrent_time_8) + "μs (" +
              to_string((concurrent_time_8 - non_concurrent_time) * 100 / non_concurrent_time) + "%)");

    spinner->stop();
}

int main() {
    cout << "\nBenchmarking classic sorting algorithms and their concurrent implementations \n\n";
    spinner->setInterval(100);
    spinner->setSymbols("dots");

    int arraySizes[] = { 10, 100, 1000, 10000, 100000, 1000000 };
    // int arraySizes[] = { 10, 100, 1000, 10000, 100000 };

    // Initialize heap array of size ARR_SIZE with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100000);

    sortingData.open("sortingData.csv");
    sortingData << "Sort Name,Array Size,Time Taken,Number of Threads Used" << endl;

    for (int arrSize : arraySizes) {
        int *arr = new int[arrSize];
        for (int i = 0; i < arrSize; i++) {
            arr[i] = dis(gen);
        }

        benchmarkBubbleSort(arr, arrSize);
        benchmarkMergeSort(arr, arrSize);
        benchmarkRadixSort(arr, arrSize);
    }

    vt.print(cout);

    sortingData.close();

    return 0;
}

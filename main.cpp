#include <iostream>
#include <fstream>
#include <chrono>
#include <random>

#include <sys/resource.h>
#include <unistd.h>

#include "sorting_algorithms.hpp"
#include "table.h"
#include "spinners.hpp"

using namespace std;
using namespace spinners;

ofstream sortingData;

VariadicTable<string, string, string, string, string, string, string, string, string, string> vt(
        {"Sorting Algorithm", "Array Size", "Time (non-concurrent)", "CPU Usage (non-concurrent)", "Time (2 threads)", "CPU Usage (2 threads)", "Time (4 threads)", "CPU Usage (4 threads)", "Time (8 threads)", "CPU Usage (8 threads)"}, 24);
Spinner *spinner = new Spinner();

typedef struct sortData {
    long sortTime;
    double cpuUsage;
} sortData;

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

// Calculates CPU Usage as busy time / elapsed time * 100 / number of cores
double calculateCpuUsage(struct rusage startUsage, struct rusage endUsage, double elapsedTime) {
    double start = (double) startUsage.ru_utime.tv_sec + (double) startUsage.ru_utime.tv_usec / 1000000.0
                 + (double) startUsage.ru_stime.tv_sec + (double) startUsage.ru_stime.tv_usec / 1000000.0;
    double end = (double) endUsage.ru_utime.tv_sec + (double) endUsage.ru_utime.tv_usec / 1000000.0
               + (double) endUsage.ru_stime.tv_sec + (double) endUsage.ru_stime.tv_usec / 1000000.0;
    double cpuUsage = (end - start) * 100.0 / elapsedTime / thread::hardware_concurrency();
    return cpuUsage;
}

sortData getBubbleSortData(int arr[], int arrSize, int numThreads) {
    sortData data;
    auto arr_copy = getArrCopy(arr, arrSize);
    struct rusage startUsage, endUsage;

    getrusage(RUSAGE_SELF, &startUsage);
    auto t1 = chrono::high_resolution_clock::now();
    
    if (numThreads == 1) { 
        bubbleSort(arr_copy, 0, arrSize);
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentBubbleSort(arr_copy, arrSize, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads");
    }
    
    auto t2 = chrono::high_resolution_clock::now();
    getrusage(RUSAGE_SELF, &endUsage);
    
    delete arr_copy;

    data.cpuUsage = calculateCpuUsage(startUsage, endUsage, chrono::duration_cast<chrono::duration<double>>(t2 - t1).count());
    data.sortTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    return data;
}

void benchmarkBubbleSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking bubble sort...");
    spinner->start();

    cout << "Benchmarking bubble sort..." << endl;

    sortData oneThread = getBubbleSortData(arr, arrSize, 1);
    sortingData << "Bubble Sort," << arrSize << "," << oneThread.sortTime << "," << oneThread.cpuUsage << "," << "1" << endl;
    sortData twoThreads = getBubbleSortData(arr, arrSize, 2);
    sortingData << "Bubble Sort," << arrSize << "," << twoThreads.sortTime << "," << twoThreads.cpuUsage << "," << "2" << endl;
    sortData fourThreads = getBubbleSortData(arr, arrSize, 4);
    sortingData << "Bubble Sort," << arrSize << "," << fourThreads.sortTime << "," << fourThreads.cpuUsage << "," << "4" << endl;
    sortData eightThreads = getBubbleSortData(arr, arrSize, 8);
    sortingData << "Bubble Sort," << arrSize << "," << eightThreads.sortTime << "," << eightThreads.cpuUsage << "," << "8" << endl;
    
    vt.addRow("Bubble Sort", to_string(arrSize), 
              to_string(oneThread.sortTime) + "μs",
              to_string(oneThread.cpuUsage) + "%",
              to_string(twoThreads.sortTime) + "μs (" + to_string((twoThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(twoThreads.cpuUsage) + "%",
              to_string(fourThreads.sortTime) + "μs (" + to_string((fourThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(fourThreads.cpuUsage) + "%",
              to_string(eightThreads.sortTime) + "μs (" + to_string((eightThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(eightThreads.cpuUsage) + "%");

    spinner->stop();
}

sortData getMergeSortData(int arr[], int arrSize, int numThreads) {
    sortData data;
    auto arr_copy = getArrCopy(arr, arrSize);
    struct rusage startUsage, endUsage;

    getrusage(RUSAGE_SELF, &startUsage); 
    auto t1 = chrono::high_resolution_clock::now();

    if (numThreads == 1) { 
        mergeSort(arr_copy, 0, arrSize - 1); 
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentMergeSort(arr_copy, 0, arrSize - 1, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads"); 
    }

    auto t2 = chrono::high_resolution_clock::now();
    getrusage(RUSAGE_SELF, &endUsage);

    delete arr_copy;
 
    data.cpuUsage = calculateCpuUsage(startUsage, endUsage, chrono::duration_cast<chrono::duration<double>>(t2 - t1).count());
    data.sortTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    return data;
}

void benchmarkMergeSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking merge sort...");
    spinner->start();

    cout << "Benchmarking merge sort..." << endl;

    sortData oneThread = getMergeSortData(arr, arrSize, 1);
    sortingData << "Merge Sort," << arrSize << "," << oneThread.sortTime << "," << oneThread.cpuUsage << "," << "1" << endl;
    sortData twoThreads = getMergeSortData(arr, arrSize, 2);
    sortingData << "Merge Sort," << arrSize << "," << twoThreads.sortTime << "," << twoThreads.cpuUsage << "," << "2" << endl;
    sortData fourThreads = getMergeSortData(arr, arrSize, 4);
    sortingData << "Merge Sort," << arrSize << "," << fourThreads.sortTime << "," << fourThreads.cpuUsage << "," << "4" << endl;
    sortData eightThreads = getMergeSortData(arr, arrSize, 8);
    sortingData << "Merge Sort," << arrSize << "," << eightThreads.sortTime << "," << eightThreads.cpuUsage << "," << "8" << endl;
    
    vt.addRow("Merge Sort", to_string(arrSize), 
              to_string(oneThread.sortTime) + "μs",
              to_string(oneThread.cpuUsage) + "%",
              to_string(twoThreads.sortTime) + "μs (" + to_string((twoThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(twoThreads.cpuUsage) + "%",
              to_string(fourThreads.sortTime) + "μs (" + to_string((fourThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(fourThreads.cpuUsage) + "%",
              to_string(eightThreads.sortTime) + "μs (" + to_string((eightThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(eightThreads.cpuUsage) + "%");

    spinner->stop();
}

sortData getRadixSortData(int arr[], int arrSize, int numThreads) {
    sortData data;
    auto arr_copy = getArrCopy(arr, arrSize);
    struct rusage startUsage, endUsage;
    
    getrusage(RUSAGE_SELF, &startUsage);
    auto t1 = chrono::high_resolution_clock::now();

    if (numThreads == 1) { 
        radixSort(arr_copy, 0, arrSize);
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentRadixSort(arr_copy, arrSize, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads");
    }

    auto t2 = chrono::high_resolution_clock::now();
    getrusage(RUSAGE_SELF, &endUsage);
    
    delete arr_copy;

    data.cpuUsage = calculateCpuUsage(startUsage, endUsage, chrono::duration_cast<chrono::duration<double>>(t2 - t1).count());
    data.sortTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    return data;
}

void benchmarkRadixSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking radix sort...");
    spinner->start();

    cout << "Benchmarking radix sort..." << endl;

    sortData oneThread = getRadixSortData(arr, arrSize, 1);
    sortingData << "Radix Sort," << arrSize << "," << oneThread.sortTime << "," << oneThread.cpuUsage << "," << "1" << endl;
    sortData twoThreads = getRadixSortData(arr, arrSize, 2);
    sortingData << "Radix Sort," << arrSize << "," << twoThreads.sortTime << "," << twoThreads.cpuUsage << "," << "2" << endl;
    sortData fourThreads = getRadixSortData(arr, arrSize, 4);
    sortingData << "Radix Sort," << arrSize << "," << fourThreads.sortTime << "," << fourThreads.cpuUsage << "," << "4" << endl;
    sortData eightThreads = getRadixSortData(arr, arrSize, 8);
    sortingData << "Radix Sort," << arrSize << "," << eightThreads.sortTime << "," << eightThreads.cpuUsage << "," << "8" << endl;
    
    vt.addRow("Radix Sort", to_string(arrSize), 
              to_string(oneThread.sortTime) + "μs",
              to_string(oneThread.cpuUsage) + "%",
              to_string(twoThreads.sortTime) + "μs (" + to_string((twoThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(twoThreads.cpuUsage) + "%",
              to_string(fourThreads.sortTime) + "μs (" + to_string((fourThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(fourThreads.cpuUsage) + "%",
              to_string(eightThreads.sortTime) + "μs (" + to_string((eightThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(eightThreads.cpuUsage) + "%");

    spinner->stop();
}

sortData getInsertionSortData(int arr[], int arrSize, int numThreads) {
    sortData data;
    auto arr_copy = getArrCopy(arr, arrSize);
    struct rusage startUsage, endUsage;
    
    getrusage(RUSAGE_SELF, &startUsage);
    auto t1 = chrono::high_resolution_clock::now();

    if (numThreads == 1) { 
        insertionSort(arr_copy, 0, arrSize-1);
        isSorted(arr_copy, arrSize, "Array with 1 thread");
    }
    else { 
        concurrentInsertionSort(arr_copy, arrSize, numThreads);
        isSorted(arr_copy, arrSize, "Array with " + to_string(numThreads) + " threads");
    }

    auto t2 = chrono::high_resolution_clock::now();
    getrusage(RUSAGE_SELF, &endUsage);
    
    delete arr_copy;

    data.cpuUsage = calculateCpuUsage(startUsage, endUsage, chrono::duration_cast<chrono::duration<double>>(t2 - t1).count());
    data.sortTime = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    return data;
}

void benchmarkInsertionSort(int arr[], int arrSize) {
    spinner->setText("Benchmarking insertion sort...");
    spinner->start();

    cout << "Benchmarking insertion sort..." << endl;

    sortData oneThread = getInsertionSortData(arr, arrSize, 1);
    sortingData << "Insertion Sort," << arrSize << "," << oneThread.sortTime << "," << oneThread.cpuUsage << "," << "1" << endl;
    sortData twoThreads = getInsertionSortData(arr, arrSize, 2);
    sortingData << "Insertion Sort," << arrSize << "," << twoThreads.sortTime << "," << twoThreads.cpuUsage << "," << "2" << endl;
    sortData fourThreads = getInsertionSortData(arr, arrSize, 4);
    sortingData << "Insertion Sort," << arrSize << "," << fourThreads.sortTime << "," << fourThreads.cpuUsage << "," << "4" << endl;
    sortData eightThreads = getInsertionSortData(arr, arrSize, 8);
    sortingData << "Insertion Sort," << arrSize << "," << eightThreads.sortTime << "," << eightThreads.cpuUsage << "," << "8" << endl;
    
    vt.addRow("Insertion Sort", to_string(arrSize), 
              to_string(oneThread.sortTime) + "μs",
              to_string(oneThread.cpuUsage) + "%",
              to_string(twoThreads.sortTime) + "μs (" + to_string((twoThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(twoThreads.cpuUsage) + "%",
              to_string(fourThreads.sortTime) + "μs (" + to_string((fourThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(fourThreads.cpuUsage) + "%",
              to_string(eightThreads.sortTime) + "μs (" + to_string((eightThreads.sortTime - oneThread.sortTime) * 100 / oneThread.sortTime) + "%)",
              to_string(eightThreads.cpuUsage) + "%");

    spinner->stop();
}



int main() {
    cout << "\nBenchmarking classic sorting algorithms and their concurrent implementations \n\n";
    spinner->setInterval(100);
    spinner->setSymbols("dots");

    // int arraySizes[] = { 10, 100, 1000, 10000, 100000, 1000000 };
    // int arraySizes[] = { 10, 100, 1000, 10000, 100000 };
    // int arraySizes[] = { 100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000 };
    // int arraySizes[] = { 100000 };
    // int arraySizes[] = { 10, 100, 1000, 10000, 100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000, 250000, 500000, 750000, 1000000};
    // int arraySizes[] = { 100000, 500000, 1000000 };
    int arraySizes[] = { 10, 100, 1000, 10000, 100000, 110000, 120000, 130000, 140000, 150000, 160000, 170000, 180000, 190000, 200000 };
    // Initialize heap array of size ARR_SIZE with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 100000);

    // int arraySizes[10];
    // for (int i = 0; i < 10; i++) {
    //     arraySizes[i] = dis(gen);
    // }

    sortingData.open("sortingData.csv");
    sortingData << "Sort Name,Array Size,Time Taken,CPU Usage,Number of Threads Used" << endl;

    for (int arrSize : arraySizes) {
        int *arr = new int[arrSize];
        for (int i = 0; i < arrSize; i++) {
            arr[i] = dis(gen);
        }

        benchmarkBubbleSort(arr, arrSize);
        benchmarkMergeSort(arr, arrSize);
        benchmarkRadixSort(arr, arrSize);
        benchmarkInsertionSort(arr, arrSize);
    }

    vt.print(cout);

    sortingData.close();

    return 0;
}

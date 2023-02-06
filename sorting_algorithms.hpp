#ifndef SORTING_ALGORITHMS_HPP
#define SORTING_ALGORITHMS_HPP

#include <thread>
#include <vector>

using namespace std;

template<class T>
void mergeSort(T arr[], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);
        int left = start;
        int right = mid + 1;
        int temp[end - start + 1];
        int temp_index = 0;
        while (left <= mid && right <= end) {
            if (arr[left] < arr[right]) {
                temp[temp_index] = arr[left];
                left++;
            } else {
                temp[temp_index] = arr[right];
                right++;
            }
            temp_index++;
        }
        while (left <= mid) {
            temp[temp_index] = arr[left];
            left++;
            temp_index++;
        }
        while (right <= end) {
            temp[temp_index] = arr[right];
            right++;
            temp_index++;
        }
        for (int i = start; i <= end; i++) {
            arr[i] = temp[i - start];
        }
    }
}

template<class T>
void bubbleSort(T arr[], int start, int end) {
    int n = end - start;
    T temp;
    for (int i = 0; i < n; i++) {
        for (int j = start; j < end - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

template<class T>
void concurrentBubbleSort(T arr[], int n, int num_threads) {
    vector<thread> threads;
    int chunk_size = n / num_threads;
    int start = 0;
    int end = chunk_size;

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(bubbleSort<T>, ref(arr), start, end);
        start = end;
        end += chunk_size;
    }

    for (auto &thread: threads) {
        thread.join();
    }

    // Merge the sorted chunks using merge sort
    mergeSort(arr, 0, n - 1);
}

#endif //SORTING_ALGORITHMS_HPP
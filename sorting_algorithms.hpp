#ifndef SORTING_ALGORITHMS_HPP
#define SORTING_ALGORITHMS_HPP

#include <thread>
#include <vector>

using namespace std;

// Can be parallelized
template <typename T>
void merge(T arr[], int start, int mid, int end) {
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

template<class T>
void mergeSort(T arr[], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(arr, start, mid);
        mergeSort(arr, mid + 1, end);
        merge(arr, start, mid, end);
    }
}

// Assumes that numThreads is a power of 2
template <typename T>
void concurrentMergeSort(T arr[], int start, int end, int numThreads) {
    if (start < end) {
        int mid = (start + end) / 2;
        if (numThreads > 1) {
            std::vector<std::thread> threads(2);
            int numThreadsLeft = numThreads / 2;
            int numThreadsRight = numThreads - numThreadsLeft;
            threads[0] = std::thread(concurrentMergeSort<T>, arr, start, mid, numThreadsLeft);
            threads[1] = std::thread(concurrentMergeSort<T>, arr, mid + 1, end, numThreadsRight);
            for (auto& thread : threads) {
                thread.join();
            }
        } else {
            concurrentMergeSort(arr, start, mid, 1);
            concurrentMergeSort(arr, mid + 1, end, 1);
        }
        merge(arr, start, mid, end);
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
        end = min(end + chunk_size, n);
        // end += chunk_size;
    }
    // for (int i = 0; i < num_threads - 1; i++) {
    // threads.emplace_back(bubbleSort<T>, ref(arr), i*chunk_size, (i+1)*chunk_size);
    // }
    // threads.emplace_back(bubbleSort<T>, ref(arr), (num_threads-1)*chunk_size, n);

    for (auto &thread: threads) {
        thread.join();
    }

    // Merge the sorted chunks using merge
    // Start at i = 1 because the first chunk is already sorted
    // Each iteration of the loop merges two adjacent chunks
    // First chunk starts at 0 and ends at (i * chunk_size) - 1
    // Second chunk starts at (i * chunk_size) and ends at (i + 1) * chunk_size - 1
    for (int i = 1; i < num_threads; i++) {
        // merge(arr, 0, (i * chunk_size) - 1, (i + 1) * chunk_size - 1);
        int mid = i * chunk_size - 1;
        int right = min((i + 1) * chunk_size - 1, n - 1);
        merge(arr, 0, mid, right);
    }
    
    // // Merge the sorted chunks using merge
    // // Each iteration of the loop merges all adjacent chunks
    // // The size of the chunks being merged doubles at each iteration
    // for (int size = chunk_size; size < n; size *= 2) {
    //     for (int i = 0; i < n; i += 2 * size) {
    //         int left = i;
    //         int mid = min(i + size - 1, n - 1);
    //         int right = min(i + 2 * size - 1, n - 1);
    //         merge(arr, left, mid, right);
    //     }
    // }

    // // Merge the sorted chunks
    // int merge_count = num_threads / 2;
    // while (merge_count > 0) {
    //     int chunk_start = 0;
    //     for (int i = 0; i < merge_count; i++) {
    //         int chunk_end = chunk_start + chunk_size * 2 - 1;
    //         if (chunk_end >= n) {
    //             chunk_end = n - 1;
    //         }
    //         merge(arr, chunk_start, chunk_start + chunk_size - 1, chunk_end);
    //         chunk_start = chunk_end + 1;
    //     }
    //     if (merge_count == 1 && chunk_start < n) {
    //         merge(arr, 0, chunk_start - 1, n - 1);
    //     }
    //     merge_count = merge_count / 2 + merge_count % 2;
    // }
    
    // // merge sub-arrays iteratively
    // for (int size = chunk_size; size < n; size *= 2) {
    //     for (int left = 0; left < n - size; left += 2*size) {
    //         int mid = left + size - 1;
    //         int right = min(left + 2*size - 1, n - 1);
    //         merge(arr, left, mid, right);
    //     }
    // }
}

#endif //SORTING_ALGORITHMS_HPP
#ifndef SORTING_ALGORITHMS_HPP
#define SORTING_ALGORITHMS_HPP

#include <thread>
#include <vector>

using namespace std;

// Can be parallelized
template <class T>
void merge(T arr[], int start, int mid, int end) {
    int left = start;
    int right = mid + 1;
    int temp[end - start + 1];
    int tempIndex = 0;

    while (left <= mid && right <= end) {
        if (arr[left] < arr[right]) {
            temp[tempIndex] = arr[left];
            left++;
        } else {
            temp[tempIndex] = arr[right];
            right++;
        }
        tempIndex++;
    }

    while (left <= mid) {
        temp[tempIndex] = arr[left];
        left++;
        tempIndex++;
    }

    while (right <= end) {
        temp[tempIndex] = arr[right];
        right++;
        tempIndex++;
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
template <class T>
void concurrentMergeSort(T arr[], int start, int end, int numThreads) {
    if (start < end) {
        int mid = (start + end) / 2;
        
        if (numThreads > 1) {
            vector<thread> threads(2);

            int numThreadsLeft = numThreads / 2;
            int numThreadsRight = numThreads - numThreadsLeft;

            threads[0] = thread(concurrentMergeSort<T>, arr, start, mid, numThreadsLeft);
            threads[1] = thread(concurrentMergeSort<T>, arr, mid + 1, end, numThreadsRight);
            
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
    int n = end - start + 1;
    for (int i = 0; i < n; i++) {
        for (int j = start; j < end - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

template<class T>
void concurrentBubbleSort(T arr[], int n, int numThreads) {
    vector<thread> threads;

    int chunkSize = (int)floor(n / numThreads);

    for (int i = 0; i < numThreads - 1; i++) {
        int start = i * chunkSize;
        int end = start + chunkSize;
        threads.emplace_back(bubbleSort<T>, ref(arr), start, end);
    }
    threads.emplace_back(bubbleSort<T>, ref(arr), (numThreads - 1) * chunkSize, n);

    for (auto &thread: threads) {
        thread.join();
    }

    // Similar to merging in merge sort, but we merge chunks of size chunk_size
    // Doubles the size of the chunks each iteration
    for (int size = chunkSize; size < n; size *= 2) {
        for (int i = 0; i < n - size; i += 2 * size) {
            int left = i;
            int mid = i + size - 1;
            int right = min(i + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

void radixSort(int arr[], int start, int end) {
    // Get max element in array
    int max = arr[start];
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    int n = end - start;
    // Iterate through all digits of max element
    for (int exp = 1; max / exp > 0; exp *= 10) {
        int output[n];
        int count[10] = {0};

        // Count the number of elements with each digit
        for (int i = start; i < end; i++) {
            int digit = (arr[i] / exp) % 10;
            count[digit]++;
        }

        // Cumulative sum of count array
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = end - 1; i >= start; i--) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        // Copy output array to arr
        for (int i = start; i < end; i++) {
            arr[i] = output[i - start];
        }
    }
}

void concurrentRadixSort(int arr[], int n, int numThreads) {
    vector<thread> threads;

    int chunkSize = (int)floor(n / numThreads);

    for (int i = 0; i < numThreads - 1; i++) {
        int start = i * chunkSize;
        int end = min(start + chunkSize, n);
        threads.emplace_back(radixSort, ref(arr), start, end);
    }
    threads.emplace_back(radixSort, ref(arr), (numThreads - 1) * chunkSize, n);
    
    for (auto &thread: threads) {
        thread.join();
    }

    // Similar to merging in merge sort, but we merge chunks of size chunkSize
    // Doubles the size of the chunks each iteration
    for (int size = chunkSize; size < n; size *= 2) {
        for (int i = 0; i < n - size; i += 2 * size) {
            int left = i;
            int mid = i + size - 1;
            int right = min(i + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

void insertionSort(int arr[], int start, int end) {
    int i, key, j;
    for (i = start + 1; i <= end; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[start..i-1], that are greater than key to one position ahead of their current position
        while (j >= start && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void concurrentInsertionSort(int arr[], int n, int num_threads) {
    vector<thread> threads;

    int chunkSize = (int)floor(n / num_threads);

    for (int i = 0; i < num_threads-1; i++) {
        int start = i * chunkSize;
        int end = min(start + chunkSize, n);
        threads.emplace_back(insertionSort, ref(arr), start, end - 1);
    }
    threads.emplace_back(insertionSort, ref(arr), (num_threads - 1) * chunkSize, n - 1);    

    for (auto &thread : threads) {
        thread.join();
    }
    
    // Similar to merging in merge sort, but we merge chunks of size chunkSize
    // Doubles the size of the chunks each iteration
    for (int size = chunkSize; size < n; size *= 2) {
        for (int i = 0; i < n - size; i += 2 * size) {
            int left = i;
            int mid = i + size - 1;
            int right = min(i + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

int partition(int arr[], int start, int end) {
    int pivot = arr[end];
    int i = start - 1;
    for (int j = start; j <= end - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[end]);

    return i + 1;
}

void quickSort(int arr[], int start, int end) {
    if (start < end) {
        int pivot = partition(arr, start, end);
        quickSort(arr, start, pivot - 1);
        quickSort(arr, pivot + 1, end);
    }
}

void concurrentQuickSort(int arr[], int n, int numThreads) {
    vector<thread> threads;

    int chunkSize = (int)floor(n / numThreads);

    for (int i = 0; i < numThreads - 1; i++) {
        int start = i * chunkSize;
        int end = min(start + chunkSize, n);
        threads.emplace_back(quickSort, ref(arr), start, end - 1);
    }
    threads.emplace_back(quickSort, ref(arr), (numThreads - 1) * chunkSize, n - 1);
    
    for (auto &thread : threads) {
        thread.join();
    }

    // Similar to merging in merge sort, but we merge chunks of size chunkSize
    // Doubles the size of the chunks each iteration
    for (int size = chunkSize; size < n; size *= 2) {
        for (int i = 0; i < n - size; i += 2 * size) {
            int left = i;
            int mid = i + size - 1;
            int right = min(i + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

void selectionSort(int arr[], int start, int end) {
    for (int i = start; i <= end; i++) {
        int minIndex = i;
        for (int j = i + 1; j <= end; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        swap(arr[minIndex], arr[i]);
    }
}

void concurrentSelectionSort(int arr[], int n, int numThreads) {
    vector<thread> threads;

    int chunkSize = (int)floor(n / numThreads);

    for (int i = 0; i < numThreads - 1; i++) {
        int start = i * chunkSize;
        int end = min(start + chunkSize, n);
        threads.emplace_back(selectionSort, ref(arr), start, end - 1);
    }
    threads.emplace_back(selectionSort, ref(arr), (numThreads - 1) * chunkSize, n - 1);
    
    for (auto &thread : threads) {
        thread.join();
    }

    // Similar to merging in merge sort, but we merge chunks of size chunkSize
    // Doubles the size of the chunks each iteration
    for (int size = chunkSize; size < n; size *= 2) {
        for (int i = 0; i < n - size; i += 2 * size) {
            int left = i;
            int mid = i + size - 1;
            int right = min(i + 2 * size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

#endif //SORTING_ALGORITHMS_HPP


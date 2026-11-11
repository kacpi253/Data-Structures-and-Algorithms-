

#include <iostream>
#include <chrono> 
#include <ctime>
#include <cstdio>

using namespace std;

#define ROZMIAR1 5000

#define ROZMIAR2 10000

#define ROZMIAR3 30000


struct sortResult {
    long long comparisons = 0;
    long long swaps = 0;
    double duration = 0;
};


void fillRandomArray(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = (rand() % size) + 1;
    }
}

void swapElements(int& a, int& b, sortResult& result) {
    int temp = a;
    a = b;
    b = temp;
    result.swaps++;
}

bool compareElements(int a, int b, sortResult& result) {
    result.comparisons++;
    return a > b;
}

void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void bubbleSort(int arr[], int n, sortResult& result) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (compareElements(arr[j], arr[j + 1], result)) {
                swapElements(arr[j], arr[j + 1], result);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}


void insertionSort(int arr[], int n, sortResult& result) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            result.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                result.swaps++;
                j--;
            }
            else {
                break;
            }
        }
        arr[j + 1] = key;
        result.swaps++;
    }
}



void selectionSort(int arr[], int n, sortResult& result) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            result.comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swapElements(arr[i], arr[min_idx], result);
        }
    }
}


void combSort(int arr[], int n, sortResult& result) {
    int gap = n;
    const double shrink_factor = 1.3;
    bool swapped = true;

    while (gap != 1 || swapped == true) {
        gap = floor(gap / shrink_factor);
        if (gap < 1) {
            gap = 1;
        }

        swapped = false;
        for (int i = 0; i < n - gap; i++) {
            if (compareElements(arr[i], arr[i + gap], result)) {
                swapElements(arr[i], arr[i + gap], result);
                swapped = true;
            }
        }
    }
}

void shellSort(int arr[], int n, sortResult& result) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                result.comparisons++;
                arr[j] = arr[j - gap];
                result.swaps++;
            }
            if (j >= gap) {
                result.comparisons++;
            }
            arr[j] = temp;
            result.swaps++;
        }
    }
}


void test(void (*sortF)(int arr[], int n, sortResult& result), string nazwa, int arr[], int n) {
    clock_t start, stop;
    sortResult result;
   

    start = clock();
    sortF(arr, n, result);
    stop = clock();
    result.duration = (double)(stop - start) / CLOCKS_PER_SEC;
    cout << "Czas posortowania tablicy  " << n << " elementowej metoda sortowania " << nazwa << " wynosi: " << result.duration 
        << "s, ilosc porownan to: " << result.comparisons << " a ilosc zamian to: " << result.swaps << "." << endl;
}

int main()
{

    int tab1[ROZMIAR1];
    int tab12[ROZMIAR1];
    int tab13[ROZMIAR1];
    int tab14[ROZMIAR1];
    int tab15[ROZMIAR1];
    fillRandomArray(tab1, ROZMIAR1);
    copy(tab1, tab1 + ROZMIAR1, tab12);
    copy(tab1, tab1 + ROZMIAR1, tab13);
    copy(tab1, tab1 + ROZMIAR1, tab14);
    copy(tab1, tab1 + ROZMIAR1, tab15);

    test(bubbleSort, "Sortowanie Babelkowe", tab1, ROZMIAR1);
    test(insertionSort, "Sortowanie InsertionSort", tab12, ROZMIAR1);
    test(selectionSort, "Sortowanie SelectionSort", tab13, ROZMIAR1);
    test(combSort, "Sortowanie combSort", tab14, ROZMIAR1);
    test(shellSort, "Sortowanie ShellSort", tab15, ROZMIAR1);
    cout << endl;

    int tab2[ROZMIAR2];
    int tab22[ROZMIAR2];
    int tab23[ROZMIAR2];
    int tab24[ROZMIAR2];
    int tab25[ROZMIAR2];
    fillRandomArray(tab2, ROZMIAR2);

    copy(tab2, tab2 + ROZMIAR2, tab22);
    copy(tab2, tab2 + ROZMIAR2, tab23);
    copy(tab2, tab2 + ROZMIAR2, tab24);
    copy(tab2, tab2 + ROZMIAR2, tab25);
    test(bubbleSort, "Sortowanie Babelkowe", tab2, ROZMIAR2);
    test(insertionSort, "Sortowanie InsertionSort", tab22, ROZMIAR2);
    test(selectionSort, "Sortowanie SelectionSort", tab23, ROZMIAR2);
    test(combSort, "Sortowanie combSort", tab24, ROZMIAR2);
    test(shellSort, "Sortowanie shellSort", tab25, ROZMIAR2);
    cout << endl;

    int tab3[ROZMIAR3];
    int tab32[ROZMIAR3];
    int tab33[ROZMIAR3];
    int tab34[ROZMIAR3];
    int tab35[ROZMIAR3];
    fillRandomArray(tab3, ROZMIAR3);
    copy(tab3, tab3 + ROZMIAR3, tab32);
    copy(tab3, tab3 + ROZMIAR3, tab33);
    copy(tab3, tab3 + ROZMIAR3, tab34);
    copy(tab3, tab3 + ROZMIAR3, tab35);

    test(bubbleSort, "Sortowanie Babelkowe", tab3, ROZMIAR3);
    test(insertionSort, "Sortowanie InsertionSort", tab32, ROZMIAR3);
    test(selectionSort, "Sortowanie SelectionSort", tab33, ROZMIAR3);
    test(combSort, "Sortowanie combSort", tab34, ROZMIAR3);
    test(shellSort, "Sortowanie shellSort", tab35, ROZMIAR3);

  
}


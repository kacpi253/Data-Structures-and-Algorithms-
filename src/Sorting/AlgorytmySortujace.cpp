#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <cstdlib>

using namespace std;

//Pomocnicze do bucket sort
void insertionSort(vector<int>& bucket) {
    for (size_t i = 1; i < bucket.size(); ++i) {
        int key = bucket[i];
        int j = (int)i - 1;

        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

//Pomocnicza dla hybrydowego
void insertionSort(int tab[], int lewy, int prawy) {
    for (int i = lewy + 1; i <= prawy; i++) {
        int klucz = tab[i];
        int j = i - 1;

        while (j >= lewy && tab[j] > klucz) {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = klucz;
    }
}

//bucket sort
void bucketSort(int arr[], int n, int max_val) {
    if (n <= 1 || max_val <= 0) return;
    const int NUM_BUCKETS = 10;

    int bucket_size = (max_val + NUM_BUCKETS - 1) / NUM_BUCKETS;

    vector<int> b[NUM_BUCKETS];

    for (int i = 0; i < n; i++) {
        int bi = arr[i] / bucket_size;

        if (bi >= NUM_BUCKETS) bi = NUM_BUCKETS - 1;
        b[bi].push_back(arr[i]);
    }

    for (int i = 0; i < NUM_BUCKETS; i++) {
        insertionSort(b[i]); 
    }

    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int val : b[i]) {
            arr[index++] = val;
        }
    }
}

//sortowanie cz¹stkowe
void sortowanieCzastkowe(int tab[], int n, int k) {
    for (int i = 0; i < k; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[minIndex]) {
                minIndex = j;
            }
        }
        swap(tab[i], tab[minIndex]);
    }
}

// dzielenie dla quicksorta
int podzielTablice(int tab[], int lewy, int prawy) {
    int pivot = tab[prawy];
    int i = (lewy - 1);
    for (int j = lewy; j < prawy; j++) {
        if (tab[j] < pivot) {
            i++;
            swap(tab[i], tab[j]);
        }
    }
    swap(tab[i + 1], tab[prawy]);
    return (i + 1);
}

//hybrydowe sortwanie
void sortowanieHybrydowe(int tab[], int lewy, int prawy) {

    if (lewy < prawy) {
        if (prawy - lewy < 10) {
            insertionSort(tab, lewy, prawy); 
        }
        else {
            int pivotIndex = podzielTablice(tab, lewy, prawy);
            sortowanieHybrydowe(tab, lewy, pivotIndex - 1);
            sortowanieHybrydowe(tab, pivotIndex + 1, prawy);
        }
    }
}

//Sortowanie w miejscu
void sortowanieWMiejscu(int tab[], int n) {
    for (int i = 1; i < n; i++) {
        int klucz = tab[i];
        int j = i - 1;

        while (j >= 0 && tab[j] > klucz) {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = klucz;
    }
}

//sortowanie neistandardowych danych
struct Student {
    int id;
    string imie;
    double ocena; 
};

void sortujStudentow(Student tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (tab[j].ocena < tab[j + 1].ocena) {
                swap(tab[j], tab[j + 1]);
            }
        }
    }
}


// Pomocnicza funkcja do kopiowania tablicy int
void kopiujTablice(int zrodlo[], int cel[], int n) {
    for (int i = 0; i < n; i++) {
        cel[i] = zrodlo[i];
    }
}

int main()
{
    srand(time(0));
    clock_t start, stop;

  
    const int N_BIG = 100000;
    const int N_SLOW = 10000; // Mniejszy rozmiar dla wolnych O(N^2) algorytmów
    const int MAX_VALUE = 10000;

    int oryginalna[N_BIG]; 
    int robocza[N_BIG];

    // Wype³nienie tablicy
    for (int i = 0; i < N_BIG; i++) {
        oryginalna[i] = (rand() % MAX_VALUE) + 1;
    }

    cout << "--- START TESTOW NA INT ---" << endl;

    // A) BUCKET SORT (N_BIG)
    cout << "\n--- BUCKET SORT ---" << endl;
    kopiujTablice(oryginalna, robocza, N_BIG);
    start = clock();
    bucketSort(robocza, N_BIG, MAX_VALUE);
    stop = clock();
    double duration_bucket = (double)(stop - start) / CLOCKS_PER_SEC;
    cout << "Czas sortowania " << N_BIG << " elementow: " << duration_bucket << "s" << endl;
    cout << "Sprawdzenie (Max wartosc): " << robocza[N_BIG - 1] << "\n";

    // B) HYBRYDOWY SORT (N_BIG)
    cout << "\n--- SORTOWANIE HYBRYDOWE ---" << endl;
    kopiujTablice(oryginalna, robocza, N_BIG);
    start = clock();
    sortowanieHybrydowe(robocza, 0, N_BIG - 1);
    stop = clock();
    double duration_hybrid = (double)(stop - start) / CLOCKS_PER_SEC;
    cout << "Czas sortowania " << N_BIG << " elementow: " << duration_hybrid << "s" << endl;
    cout << "Sprawdzenie (Max wartosc): " << robocza[N_BIG - 1] << "\n";

    // C) SORTOWANIE CZ¥STKOWE (N_BIG)
    cout << "\n--- SORTOWANIE CZ¥STKOWE (k=1000) ---" << endl;
    kopiujTablice(oryginalna, robocza, N_BIG);
    int k_partial = 1000;
    start = clock();
    sortowanieCzastkowe(robocza, N_BIG, k_partial);
    stop = clock();
    double duration_partial = (double)(stop - start) / CLOCKS_PER_SEC;
    cout << "Czas znalezienia " << k_partial << " najmniejszych z " << N_BIG << ": " << duration_partial << "s" << endl;
    cout << "Najwiekszy z top " << k_partial << " elementow: " << robocza[k_partial - 1] << "\n";

    // D) SORTOWANIE W MIEJSCU (Insertion Sort) - U¿ywamy mniejszej tablicy
    int oryginalna_slow[N_SLOW];
    int robocza_slow[N_SLOW];
    for (int i = 0; i < N_SLOW; i++) { oryginalna_slow[i] = oryginalna[i]; }

    cout << "\n--- SORTOWANIE W MIEJSCU (Insertion Sort, N=" << N_SLOW << ") ---" << endl;
    kopiujTablice(oryginalna_slow, robocza_slow, N_SLOW);
    start = clock();
    sortowanieWMiejscu(robocza_slow, N_SLOW);
    stop = clock();
    double duration_inplace = (double)(stop - start) / CLOCKS_PER_SEC;
    cout << "Czas sortowania " << N_SLOW << " elementow: " << duration_inplace << "s" << endl;
    cout << "Sprawdzenie (Max wartosc): " << robocza_slow[N_SLOW - 1] << "\n";

    // E) SORTOWANIE NIESTANDARDOWE
    cout << "\n--- SORTOWANIE STUDENTOW ---" << endl;
    Student studenci[] = {
        {101, "Anna", 3.0}, {102, "Marek", 5.0}, {103, "Kasia", 4.5}
    };
    sortujStudentow(studenci, 3);
    for (int i = 0; i < 3; i++) {
        cout << studenci[i].imie << " (ID: " << studenci[i].id << ", Ocena: " << studenci[i].ocena << ")" << endl;
    }

    cout << "\n--- KONIEC TESTOW ---" << endl;

    return 0;
}
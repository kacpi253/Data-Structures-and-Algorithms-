#include <iostream>
#include <vector>
#include <algorithm> // Do sortowania

using namespace std;

// Struktura krawêdzi
struct Krawedz {
    int u, v; // Wierzcho³ki
    int waga; // Koszt

    // Operator aby sortowanie bylo na podstawie wagi
    bool operator<(const Krawedz& inna) const {
        return waga < inna.waga;
    }
};

// Struktura do obs³ugi zbiorów roz³¹cznych (Find-Union)
struct ZbiorRozlaczny {
    vector<int> rodzic;

    ZbiorRozlaczny(int n) {
        rodzic.resize(n);
        for (int i = 0; i < n; i++)
            rodzic[i] = i; // Na pocz¹tku ka¿dy jest swoim rodzicem
    }

    int znajdz(int i) {
        if (rodzic[i] == i)
            return i;
        return znajdz(rodzic[i]);
    }

    // laczenie dwoch zbiorow
    void polacz(int i, int j) {
        int r1 = znajdz(i);
        int r2 = znajdz(j);
        if (r1 != r2) {
            rodzic[r1] = r2;
        }
    }
};

char intToChar(int i) { return (char)('A' + i); }

int main() {
    cout << "\n--- ZADANIE 2: Kruskal (Wyspy) ---" << endl;

    vector<Krawedz> krawedzie;


    // Poziome
    krawedzie.push_back({ 0, 1, 1 }); // A-B
    krawedzie.push_back({ 2, 3, 3 }); // C-D
    krawedzie.push_back({ 4, 5, 5 }); // E-F
    // Pionowe i skoœne
    krawedzie.push_back({ 0, 2, 4 }); // A-C
    krawedzie.push_back({ 1, 2, 2 }); // B-C (przek¹tna)
    krawedzie.push_back({ 1, 3, 7 }); // B-D
    krawedzie.push_back({ 2, 4, 1 }); // C-E
    krawedzie.push_back({ 3, 5, 5 }); // D-F

    // 1. Sortowanie krawêdzi rosn¹co
    sort(krawedzie.begin(), krawedzie.end());

    ZbiorRozlaczny zbiory(6); // 6 wysp
    int kosztCalkowity = 0;
    int licznikMostow = 0;

    cout << "Kolejnosc budowania mostow:" << endl;

    for (const auto& k : krawedzie) {
        int u = k.u;
        int v = k.v;

        // Jeœli s¹ w ró¿nych zbiorach, dodanie krawêdzi nie zrobi cyklu
        if (zbiory.znajdz(u) != zbiory.znajdz(v)) {
            cout << "Most " << intToChar(u) << "-" << intToChar(v)
                << " (Koszt: " << k.waga << ")" << endl;

            kosztCalkowity += k.waga;
            zbiory.polacz(u, v);
            licznikMostow++;
        }
    }

    cout << "Minimalny koszt odbudowy: " << kosztCalkowity << endl;

    return 0;
}
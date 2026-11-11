#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class SiecMiast {
private:
    int V;
    vector<vector<int>> macierz;
    const int INF = 1e9;

    char toChar(int i) const { return (char)('A' + i); }

public:
    // Konstruktor inicjalizuj¹cy macierz zerami
    SiecMiast(int wierzcholki) : V(wierzcholki) {
        macierz.assign(V, vector<int>(V, 0));
    }

    // Metoda dodaj¹ca po³¹czenie
    void dodajPolaczenie(int u, int v, int waga) {
        macierz[u][v] = waga;
        macierz[v][u] = waga;
    }

    // G³ówna metoda algorytmu Prima
    void obliczIMST() {
        vector<int> klucz(V, INF);
        vector<int> rodzic(V, -1);
        vector<bool> wMST(V, false);

        klucz[0] = 0;

        for (int i = 0; i < V; i++) {
            // Szukanie wierzcho³ka o min. kluczu
            int u = -1;
            for (int v = 0; v < V; v++) {
                if (!wMST[v] && (u == -1 || klucz[v] < klucz[u]))
                    u = v;
            }

            if (u == -1) break; // Graf niespójny
            wMST[u] = true;

            // Aktualizacja s¹siadów
            for (int v = 0; v < V; v++) {
                if (macierz[u][v] && !wMST[v] && macierz[u][v] < klucz[v]) {
                    klucz[v] = macierz[u][v];
                    rodzic[v] = u;
                }
            }
        }

        wyswietlWynik(rodzic);
    }

    void wyswietlWynik(const vector<int>& rodzic) {
        int suma = 0;
        cout << "--- Minimalne Drzewo Rozpinaj¹ce (Prim) ---\n";
        for (int i = 1; i < V; i++) {
            if (rodzic[i] != -1) {
                cout << "Polaczenie: " << toChar(rodzic[i]) << " - " << toChar(i)
                    << " | Koszt: " << macierz[i][rodzic[i]] << "\n";
                suma += macierz[i][rodzic[i]];
            }
        }
        cout << "-------------------------------------------\n";
        cout << "Calkowity koszt sieci: " << suma << endl;
    }
};

int main() {
    SiecMiast miasto(6);

    // Budujemy graf
    miasto.dodajPolaczenie(0, 1, 6); // A-B
    miasto.dodajPolaczenie(0, 2, 1); // A-C
    miasto.dodajPolaczenie(0, 3, 5); // A-D
    miasto.dodajPolaczenie(1, 2, 5); // B-C
    miasto.dodajPolaczenie(1, 4, 3); // B-E
    miasto.dodajPolaczenie(2, 3, 5); // C-D
    miasto.dodajPolaczenie(2, 4, 6); // C-E
    miasto.dodajPolaczenie(2, 5, 4); // C-F
    miasto.dodajPolaczenie(3, 5, 2); // D-F
    miasto.dodajPolaczenie(4, 5, 6); // E-F

    miasto.obliczIMST();

    return 0;
}
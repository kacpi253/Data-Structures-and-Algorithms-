#include <iostream>
#include <vector>
#include <queue> 
#include <algorithm> 

using namespace std;

class Wezel {
public:
    int id;                      //identyfikator w�z��
    vector<Wezel*> sasiedzi;     //sasiedzi
    bool odwiedzony;             // 

    Wezel(int k) : id(k), odwiedzony(false) {}

    //tworzenie krawedzi
    void dodajSasiada(Wezel* sasiad) {
        this->sasiedzi.push_back(sasiad);

    }
};


class Graf {
private:
    vector<Wezel*> wszystkieWezly; //lista wszystkich wezlow

    // resetowanie flag
    void resetujOdwiedzone() {
        for (Wezel* w : wszystkieWezly) {
            w->odwiedzony = false;
        }
    }

public:
    // Destruktor - sprz�tanie pami�ci
    ~Graf() {
        for (Wezel* w : wszystkieWezly) {
            delete w;
        }
    }

    // Tworzenie nowego w�z�a i dodawanie go do grafu
    Wezel* stworzWezel(int id) {
        Wezel* nowy = new Wezel(id);
        wszystkieWezly.push_back(nowy);
        return nowy;
    }

    // ��czenie dw�ch w�z��w krawędzi�
    void dodajKrawedz(Wezel* w1, Wezel* w2) {
        w1->dodajSasiada(w2);
        w2->dodajSasiada(w1);                   // Graf nieskierowany: A->B oraz B->A
    }

    // Algorytm przechodzenia wszerz BFS
    void BFS(Wezel* start) {
        if (start == nullptr) return;

        resetujOdwiedzone(); 

        queue<Wezel*> kolejka;

        // 1. Odwiedzamy start
        start->odwiedzony = true;
        kolejka.push(start);

        cout << "BFS (Wszerz): ";

        while (!kolejka.empty()) {
            // Pobierz element z pocz�tku kolejki
            Wezel* aktualny = kolejka.front();
            kolejka.pop();

            cout << aktualny->id << " ";

            // przejscie po wszystkich sasiadach
            for (Wezel* sasiad : aktualny->sasiedzi) {
                if (!sasiad->odwiedzony) {
                    sasiad->odwiedzony = true;
                    kolejka.push(sasiad);
                }
            }
        }
        cout << endl;
    }

    // przechodzenie wzdluz DFS
    void DFS(Wezel* start) {
        if (start == nullptr) return;

        resetujOdwiedzone();
        cout << "DFS (Wzdloz): ";
        dfsRekurencja(start);
        cout << endl;
    }

private:
    void dfsRekurencja(Wezel* w) {
        // wypisywanie i flagowanie
        w->odwiedzony = true;
        cout << w->id << " ";

        // rekurencja w kazdego sasiada
        for (Wezel* sasiad : w->sasiedzi) {
            if (!sasiad->odwiedzony) {
                dfsRekurencja(sasiad);
            }
        }
    }
};

int main() {
    Graf g;


    Wezel* A = g.stworzWezel(1); // Liczby zamiast liter 1-A ..
    Wezel* B = g.stworzWezel(2);
    Wezel* C = g.stworzWezel(3);
    Wezel* D = g.stworzWezel(4);
    Wezel* E = g.stworzWezel(5);

    /* wizualizacja grafu
         1 -- 2
         |    |
         3 -- 4 -- 5
    */

    // ��czenie krawedzi
    g.dodajKrawedz(A, B); // 1-2
    g.dodajKrawedz(A, C); // 1-3
    g.dodajKrawedz(B, D); // 2-4
    g.dodajKrawedz(C, D); // 3-4
    g.dodajKrawedz(D, E); // 4-5

    
    g.BFS(A);
    g.DFS(A);

    return 0;
}
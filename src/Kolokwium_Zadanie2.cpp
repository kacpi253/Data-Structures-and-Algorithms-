#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Wezel {
    int id;
    int kolor; // -1: nieodwiedzony, 0: Grupa A, 1: Grupa B
    vector<Wezel*> sasiedzi; 

    Wezel(int _id) : id(_id), kolor(-1) {}
};

class Graf {
private:
    vector<Wezel*> listaWezlow; 

public:
    Graf(int n) {
        listaWezlow.push_back(nullptr);   // przypisanie indeksu 0
        for (int i = 1; i <= n; i++) {
            listaWezlow.push_back(new Wezel(i));
        }
    }

    void dodajKrawedz(int u, int v) {
        // Dodajemy wskaŸnik na v do listy s¹siadów u (i odwrotnie)
        listaWezlow[u]->sasiedzi.push_back(listaWezlow[v]);
        listaWezlow[v]->sasiedzi.push_back(listaWezlow[u]);
    }

    
    void sprawdzDwudzielnosc(int n) {
        bool czyMozliwe = true;

        //iteracja po wszystkich wierzcholkach, jesli trafimy na ten bez koloru to nadajemy mu go

        for (int i = 1; i <= n; i++) {
            if (listaWezlow[i]->kolor == -1) {
                if (bfs(listaWezlow[i]) == false) {         
                    czyMozliwe = false;
                    break;
                }
            }
        }

        if (czyMozliwe) {
            cout << "YES" << endl;
            wypiszZbiory(n);
        }
        else {
            cout << "NIE" << endl;
        }
    }

private:
    
    bool bfs(Wezel* start) {
        queue<Wezel*> kolejka;

        start->kolor = 0; // Zaczynamy od koloru 0
        kolejka.push(start);

        while (!kolejka.empty()) {
            Wezel* obecny = kolejka.front();
            kolejka.pop();

            // Przegl¹damy s¹siadów 
            for (int i = 0; i < obecny->sasiedzi.size(); i++) {
                Wezel* sasiad = obecny->sasiedzi[i];

                if (sasiad->kolor == -1) {
                    // nadajemy przeciwny kolor 
                    sasiad->kolor = 1 - obecny->kolor;
                    kolejka.push(sasiad);
                }
                else if (sasiad->kolor == obecny->kolor) {
                                        //jesli sasiad ma kolor ten co obecny to nie jest dwudzielny
                    return false;
                }
            }
        }
        return true;
    }

    void wypiszZbiory(int n) {
        vector<int> A, B;
        for (int i = 1; i <= n; i++) {
            if (listaWezlow[i]->kolor == 0) A.push_back(i);
            else B.push_back(i);
        }

        sort(A.begin(), A.end());
        sort(B.begin(), B.end());

        cout << "A: ";                     
        for (int x : A) cout << x << " ";
        cout << endl;

        cout << "B: ";        
        for (int x : B) cout << x << " ";
        cout << endl;
    }
};

int main() {
    int n, m;

   
    if (cin >> n >> m) {
        Graf g(n); // stworzenie grafu

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            g.dodajKrawedz(u, v);
        }

        g.sprawdzDwudzielnosc(n);
    }

    return 0;
}
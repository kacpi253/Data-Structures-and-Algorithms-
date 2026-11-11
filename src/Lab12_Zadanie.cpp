#include <iostream>

using namespace std;

struct Wezel {
    int wartosc;
    Wezel* nastepny;
    Wezel(int val) : wartosc(val), nastepny(nullptr) {}
};

class ZadanieKolokwium {
public:
    // Metoda zgodna ze stylem z lab6.cpp (zwraca wskaŸnik)
    static Wezel* dodajElement(Wezel* glowa, int wartosc) {
        Wezel* nowy = new Wezel(wartosc);
        if (glowa == nullptr) return nowy;

        Wezel* temp = glowa;
        while (temp->nastepny != nullptr) {
            temp = temp->nastepny;
        }
        temp->nastepny = nowy;
        return glowa;
    }

    // Sortowanie b¹belkowe na wartoœciach (bezpieczne i proste)
    static void sortujListe(Wezel* glowa) {
        if (glowa == nullptr || glowa->nastepny == nullptr) return;
        bool zamiana;
        do {
            zamiana = false;
            Wezel* obecny = glowa;
            while (obecny->nastepny != nullptr) {
                if (obecny->wartosc > obecny->nastepny->wartosc) {
                    swap(obecny->wartosc, obecny->nastepny->wartosc);
                    zamiana = true;
                }
                obecny = obecny->nastepny;
            }
        } while (zamiana);
    }

    // Pomocnicza do sprawdzania d³ugoœci segmentu
    static bool czyMoznaOdwrocic(Wezel* start, int k) {
        int licznik = 0;
        while (start != nullptr && licznik < k) {
            start = start->nastepny;
            licznik++;
        }
        return licznik == k;
    }

    // G³ówna logika odwracania segmentami
    static Wezel* odwrocSegmentami(Wezel* glowa, int k) {
        if (glowa == nullptr || k <= 1) return glowa;

        Wezel* dummy = new Wezel(0); // Atrapa u³atwia zarz¹dzanie now¹ g³ow¹
        dummy->nastepny = glowa;
        Wezel* przedGrupa = dummy;
        Wezel* startGrupy = glowa;

        while (startGrupy != nullptr) {
            if (!czyMoznaOdwrocic(startGrupy, k)) break;

            // Odwracanie wskaŸników wewn¹trz segmentu k
            Wezel* prev = nullptr;
            Wezel* curr = startGrupy;
            for (int i = 0; i < k; i++) {
                Wezel* nastepnyNode = curr->nastepny;
                curr->nastepny = prev;
                prev = curr;
                curr = nastepnyNode;
            }

            // £¹czenie odwróconego segmentu z reszt¹ listy
            Wezel* koniecGrupy = startGrupy;
            przedGrupa->nastepny = prev;
            koniecGrupy->nastepny = curr;

            // Przesuniêcie do nastêpnego segmentu
            przedGrupa = koniecGrupy;
            startGrupy = curr;
        }

        Wezel* wynik = dummy->nastepny;
        delete dummy;
        return wynik;
    }

    static void wypisz(Wezel* g) {
        while (g) {
            cout << g->wartosc << (g->nastepny ? " -> " : "");
            g = g->nastepny;
        }
        cout << endl;
    }
};

int main() {
    Wezel* lista = nullptr;

    // U¿ywamy przypisania, tak jak w BSTNode::Insert w Twoim lab6.cpp
    lista = ZadanieKolokwium::dodajElement(lista, 5);
    lista = ZadanieKolokwium::dodajElement(lista, 2);
    lista = ZadanieKolokwium::dodajElement(lista, 8);
    lista = ZadanieKolokwium::dodajElement(lista, 1);
    lista = ZadanieKolokwium::dodajElement(lista, 3);
    lista = ZadanieKolokwium::dodajElement(lista, 7);
    lista = ZadanieKolokwium::dodajElement(lista, 4);
    lista = ZadanieKolokwium::dodajElement(lista, 6);

    cout << "Start: "; ZadanieKolokwium::wypisz(lista);

    ZadanieKolokwium::sortujListe(lista);
    cout << "Posortowana: "; ZadanieKolokwium::wypisz(lista);

    lista = ZadanieKolokwium::odwrocSegmentami(lista, 3);
    cout << "Wynik (k=3): "; ZadanieKolokwium::wypisz(lista);

    return 0;
}
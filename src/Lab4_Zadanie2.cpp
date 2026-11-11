
#include <iostream>
using namespace std;

struct PatientNode {
    int client_id;
    int priority;
    PatientNode* next;
    PatientNode(int id, int prio) : client_id(id), priority(prio), next(nullptr) {}
};

class PriorityQueue {
private:
    PatientNode* front; // WskaŸnik na pocz¹tek kolejki (do obs³ugi)
   
public:
    PriorityQueue() : front(nullptr) {
        cout << "Utworzono pusta kolejkê sklepowa." << endl;
    }

    ~PriorityQueue() {
        cout << "\n--- Sprzatanie kolejki (Destruktor) ---" << endl;
        while (!is_empty()) {
            obsluz_klienta(false); // Wywo³ujemy obs³ugê bez komunikatu
        }
        cout << "Kolejka zniszczona." << endl;
    }

    bool is_empty() {
        return front == nullptr;
    }

    // Dodaje klienta na koniec kolejki (operacja enqueue)
    void dodaj_klienta(int client_id, int priority) {
        PatientNode* new_node = new PatientNode(client_id, priority); // Alokacja pamiêci

    
        if (is_empty() || new_node->priority < front->priority) {
            new_node->next = front;
            front = new_node;
            
        }                   //wstawianie klientow w posortowany sposob
        else {
            PatientNode *current = front;

            while (current->next != nullptr && current->next->priority <= new_node->priority) {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
        cout << "[+] Klient " << client_id << " o priorytecie: " << priority << " dolaczyl do kolejki." << endl;
    }

    // Obs³uguje klienta, usuwaj¹c go z pocz¹tku kolejki (operacja dequeue)     verbose aby wyciszyæ komuniakty
    void obsluz_klienta(bool verbose = true) {
        if (is_empty()) {
            if (verbose) {
                cout << "[-] Kolejka jest pusta. Brak klientow do obslugi." << endl;
            }
            return;
        }
        PatientNode* served_node = front;
        int client_id = served_node->client_id;
        int priority = served_node->priority;

        front = front->next;


        // Zwalniamy pamiêæ po obs³u¿onym wêŸle
        delete served_node;

        if (verbose) {
            cout << "[->] Klient " << client_id << " o priorytecie: " << priority << " zostal obsluzony." << endl;
        }
    }

    // Wyœwietla aktualny stan kolejki od pocz¹tku do koñca
    void wyswietl_stan() {
        if (is_empty()) {
            cout << "\n=== STAN KOLEJKI ===" << endl;
            cout << "   (PUSTA)" << endl;
            cout << "====================" << endl;
            return;
        }

        cout << "\n=== STAN KOLEJKI (Szpital) ===" << endl;
        cout << "   (Najwyzszy priorytet)" << endl;
        PatientNode* current = front;
        while (current != nullptr) {
            cout << "      Pacjent " << current->client_id
                << " (Prio: " << current->priority << ")" << endl;
            current = current->next;
        }
        cout << "   (Najnizszy priorytet)" << endl;
        cout << "===============================" << endl;
    }
};


using namespace std;
int main()
{
    PriorityQueue kolejka_szpitalna;

    cout << "--- Rozpoczynamy symulacje szpitala ---" << endl;

    // --- FAZA 1: Dodajemy pacjentów ---
    cout << "\n--- Do szpitala trafia 5 pacjentów (w ró¿nej kolejnoœci) ---" << endl;

    // U¿ywamy tablic, aby pêtla 'for' by³a czysta
    int ids[] = { 101, 102, 103, 104, 105 };
    int priorities[] = { 3, 5, 1, 3, 2 }; // Ró¿ne priorytety

    for (int i = 0; i < 5; ++i) {
        kolejka_szpitalna.dodaj_klienta(ids[i], priorities[i]);
    }

    // Wyœwietlamy stan po dodaniu wszystkich
    // Oczekiwana kolejnoœæ: 103 (1), 105 (2), 101 (3), 104 (3), 102 (5)
    kolejka_szpitalna.wyswietl_stan();

    // --- FAZA 2: Obs³ugujemy 2 pacjentów ---
    cout << "\n--- Obslugujemy 2 pacjentów o najwyzszym priorytecie ---" << endl;
    kolejka_szpitalna.obsluz_klienta(); // Obs³u¿y 103 (1)
    kolejka_szpitalna.obsluz_klienta(); // Obs³u¿y 105 (2)

    // Zostaj¹: 101 (3), 104 (3), 102 (5)
    kolejka_szpitalna.wyswietl_stan();

    // --- FAZA 3: Dodajemy nowego, pilnego pacjenta ---
    cout << "\n--- Do szpitala trafia nowy pacjent KRYTYCZNY ---" << endl;
    kolejka_szpitalna.dodaj_klienta(201, 1); // Prio 1, wskoczy na pocz¹tek

    // Oczekiwana kolejka: 201 (1), 101 (3), 104 (3), 102 (5)
    kolejka_szpitalna.wyswietl_stan();

    // --- FAZA 4: Obs³ugujemy WSZYSTKICH pozosta³ych ---
    cout << "\n--- Obslugujemy WSZYSTKICH pozostalych pacjentow ---" << endl;
    while (!kolejka_szpitalna.is_empty()) {
        kolejka_szpitalna.obsluz_klienta();
    }

    // --- FAZA 5: Sprawdzenie pustej kolejki ---
    kolejka_szpitalna.wyswietl_stan();
    cout << "\n--- Proba obslugi pustej kolejki ---" << endl;
    kolejka_szpitalna.obsluz_klienta();

    return 0;

}


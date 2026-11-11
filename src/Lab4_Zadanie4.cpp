#include <iostream>

using namespace std;

// --- Struktura wezla (lista dwukierunkowa) ---
// Dodajemy pole 'priority' dla lepszego wyswietlania
struct TaskNode {
    int task_id;
    int priority; // 0 = Wysoki, 1 = Zwykly
    TaskNode* next;
    TaskNode* prev;

    TaskNode(int id, int prio) : task_id(id), priority(prio), next(nullptr), prev(nullptr) {}
};

// --- Definicja klasy Kolejki Dwustronnej (Deque) ---
class Deque {
private:
    TaskNode* front; // Wskaznik na poczatek (wysoki priorytet)
    TaskNode* rear;  // Wskaznik na koniec (zwykly priorytet)

    // Prywatna metoda pomocnicza do dodawania na poczatek
    void addFront(TaskNode* new_node) {
        if (is_empty()) {
            front = new_node;
            rear = new_node;
        }
        else {
            new_node->next = front;
            front->prev = new_node;
            front = new_node;
        }
    }

    // Prywatna metoda pomocnicza do dodawania na koniec
    void addRear(TaskNode* new_node) {
        if (is_empty()) {
            front = new_node;
            rear = new_node;
        }
        else {
            rear->next = new_node;
            new_node->prev = rear;
            rear = new_node;
        }
    }

public:
    Deque() : front(nullptr), rear(nullptr) {
        cout << "Utworzono pusta kolejke dwustronna (Deque)." << endl;
    }

    ~Deque() {
        cout << "\n--- Sprzatanie kolejki (Destruktor) ---" << endl;
        while (!is_empty()) {
            obsluz_zadanie_z_poczatku(false);
        }
        cout << "Kolejka (Deque) zniszczona." << endl;
    }

    bool is_empty() {
        return front == nullptr;
    }

    // Decyduje, gdzie dodac zadanie na podstawie priorytetu
    void dodaj_zadanie(int task_id, int priority) {
        TaskNode* new_node = new TaskNode(task_id, priority);

        if (priority == 0) { // Priorytet 0 = Wysoki -> dodaj na poczatek
            addFront(new_node);
            cout << "[^] Dodano zadanie WYSOKI priorytet (ID: " << task_id << ") na poczatek." << endl;
        }
        else { // Priorytet 1 (lub kazdy inny) = Zwykly -> dodaj na koniec
            addRear(new_node);
            cout << "[v] Dodano zadanie ZWYKLE (ID: " << task_id << ") na koniec." << endl;
        }
    }

    void obsluz_zadanie_z_poczatku(bool verbose = true) {
        if (is_empty()) {
            if (verbose) cout << "[-] Kolejka jest pusta. Brak zadan na poczatku." << endl;
            return;
        }

        TaskNode* served_node = front;
        int task_id = served_node->task_id;

        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        else {
            front->prev = nullptr;
        }

        delete served_node;
        if (verbose) cout << "[->] Obsluzono zadanie z POCZATKU (ID: " << task_id << ")." << endl;
    }

    void obsluz_zadanie_z_konca(bool verbose = true) {
        if (is_empty()) {
            if (verbose) cout << "[-] Kolejka jest pusta. Brak zadan na koncu." << endl;
            return;
        }

        TaskNode* served_node = rear;
        int task_id = served_node->task_id;

        rear = rear->prev;
        if (rear == nullptr) {
            front = nullptr;
        }
        else {
            rear->next = nullptr;
        }

        delete served_node;
        if (verbose) cout << "[<-] Obsluzono zadanie z KONCA (ID: " << task_id << ")." << endl;
    }


    // Wyswietla aktualny stan kolejki
    void wyswietl_stan() {
        if (is_empty()) {
            cout << "\n=== STAN KOLEJKI (DEQUE) ===" << endl;
            cout << "   (PUSTA)" << endl;
            cout << "============================" << endl;
            return;
        }

        cout << "\n=== STAN KOLEJKI (DEQUE) ===" << endl;
        cout << "   (Poczatek - Prio 0)" << endl;
        TaskNode* current = front;
        while (current != nullptr) {
            cout << "      Zadanie " << current->task_id
                << " (Prio: " << current->priority << ")" << endl;
            current = current->next;
        }
        cout << "   (Koniec - Prio 1)" << endl;
        cout << "============================" << endl;
    }
};

int main() {
    Deque kolejka_zadan;

    cout << "\n--- Dodajemy zadania (Prio 1) ---" << endl;
    for (int i = 0; i < 3; ++i) {
        kolejka_zadan.dodaj_zadanie(201 + i, 1); // 201, 202, 203 (Prio 1)
    }
    // Kolejka: 201(1) -> 202(1) -> 203(1)
    kolejka_zadan.wyswietl_stan();

    cout << "\n--- Wchodza zadania (Prio 0) ---" << endl;
    for (int i = 0; i < 2; ++i) {
        kolejka_zadan.dodaj_zadanie(101 + i, 0); // 101, 102 (Prio 0)
    }
    // Kolejka: 102(0) -> 101(0) -> 201(1) -> 202(1) -> 203(1)
    kolejka_zadan.wyswietl_stan();

    cout << "\n--- Obslugujemy zadania ---" << endl;
    // Obsluga pilnego (Prio 0) z poczatku
    kolejka_zadan.obsluz_zadanie_z_poczatku(); // Usuwa 102

    // Obsluga zwyklego (Prio 1) z konca
    kolejka_zadan.obsluz_zadanie_z_konca();    // Usuwa 203

    // Kolejka: 101(0) -> 201(1) -> 202(1)
    kolejka_zadan.wyswietl_stan();
    // obsluga kolejnych, pozostanie 201(1)
    kolejka_zadan.obsluz_zadanie_z_poczatku();
    kolejka_zadan.obsluz_zadanie_z_konca();
    kolejka_zadan.wyswietl_stan();

    return 0;
}
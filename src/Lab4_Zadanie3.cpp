#include <iostream>
#include <thread> // Dla this_thread::sleep_for
#include <chrono> // Dla chrono::milliseconds (bêdzie mozna zasymulowac czekanie, i widziec jak koputer udziela wkanty czasu)

using namespace std;

// --- Struktura wezla 
// Przechowuje ID procesu i jego pozostaly czas wykonania 
struct ProcessNode {
    int process_id;
    int remaining_time; // Czas wykonania
    ProcessNode* next;

    ProcessNode(int id, int time) : process_id(id), remaining_time(time), next(nullptr) {}
};

// --- Klasa Kolejki 
class RoundRobinQueue {
private:
    ProcessNode* front; // Poczatek kolejki
    ProcessNode* rear;  // Koniec kolejki

public:
    RoundRobinQueue() : front(nullptr), rear(nullptr) {
        cout << "Utworzono kolejke procesora (Round Robin)." << endl;
    }

    ~RoundRobinQueue() {
        cout << "\n--- Sprzatanie kolejki (Destruktor) ---" << endl;
        while (!is_empty()) {
            // Pobieramy wezel i go usuwamy, zwalniajac pamiec
            ProcessNode* to_delete = pobierz_proces();
            cout << "Usuwanie pozostalego procesu: " << to_delete->process_id << endl;
            delete to_delete;
        }
        cout << "Kolejka zniszczona." << endl;
    }

    bool is_empty() {
        return front == nullptr;
    }

    // Dodaje NOWY proces na koniec kolejki
    void dodaj_proces(int id, int time) {
        ProcessNode* new_node = new ProcessNode(id, time);

        if (is_empty()) {
            front = new_node;
            rear = new_node;
        }
        else {
            rear->next = new_node;
            rear = new_node;
        }
        cout << "[+] Nowy proces " << id << " (czas: " << time << ") trafia do kolejki." << endl;
    }

    // Dodaje ISTNIEJACY proces z powrotem na koniec kolejki 
    void oddaj_proces(ProcessNode* node) {
        if (node == nullptr) return;

        node->next = nullptr; // Upewniamy sie, ze jest ostatni

        if (is_empty()) {
            front = node;
            rear = node;
        }
        else {
            rear->next = node;
            rear = node;
        }
        cout << "[<] Proces " << node->process_id << " wraca na koniec kolejki." << endl;
    }

    // Pobiera proces z poczatku kolejki (NIE usuwa go z pamieci)
    // Zwraca wskaznik do pobranego wezla
    ProcessNode* pobierz_proces() {
        if (is_empty()) {
            return nullptr;
        }

        ProcessNode* served_node = front;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        // Rozlaczamy wezel z listy (wazne dla `oddaj_proces`)
        served_node->next = nullptr;
        return served_node;
    }

    // Wyswietla stan kolejki
    void wyswietl_stan() {
        if (is_empty()) {
            cout << "\n=== STAN KOLEJKI PROCESORA ===" << endl;
            cout << "   (PUSTA)" << endl;
            cout << "==============================" << endl;
            return;
        }

        cout << "\n=== STAN KOLEJKI PROCESORA ===" << endl;
        cout << "   (CPU -> Pierwszy)" << endl;
        ProcessNode* current = front;
        while (current != nullptr) {
            cout << "      Proces " << current->process_id
                << " (Pozostaly czas: " << current->remaining_time << ")" << endl;
            current = current->next;
        }
        cout << "   (Ostatni)" << endl;
        cout << "==============================" << endl;
    }
};

// --- Symulacja Round Robin ---
void symuluj_RoundRobin(RoundRobinQueue& kolejka, int kwant_czasu) {
    cout << "\n\n--- ROZPOCZYNAMY SYMULACJE (Kwant czasu = " << kwant_czasu << ") ---" << endl;

    while (!kolejka.is_empty()) {
        kolejka.wyswietl_stan();

        // 1. Pobierz proces z poczatku kolejki
        ProcessNode* biezacy_proces = kolejka.pobierz_proces();

        if (biezacy_proces == nullptr) break; // Zabezpieczenie

        cout << "\n[>>] Przetwarzam proces: " << biezacy_proces->process_id
            << " (Pozostalo: " << biezacy_proces->remaining_time << ")" << endl;

        // Symulacja "pracy" procesora
        this_thread::sleep_for(chrono::milliseconds(500));
        biezacy_proces->remaining_time -= kwant_czasu;

        if (biezacy_proces->remaining_time > 0) {
            cout << "[OK] Proces " << biezacy_proces->process_id << " zuzywa kwant czasu. Pozostalo: "
                << biezacy_proces->remaining_time << endl;
            kolejka.oddaj_proces(biezacy_proces);
        }
        else {
            cout << "[!!] Proces " << biezacy_proces->process_id << " ZAKONCZYL dzialanie." << endl;
            delete biezacy_proces;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "\n--- WSZYSTKIE PROCESY ZAKONCZONE ---" << endl;
    kolejka.wyswietl_stan();
}


int main() {
    RoundRobinQueue kolejka_procesow;

    // Ustawienie kwantu czasu procesora 
    int kwant_czasu = 3;

    cout << "--- Dodajemy procesy do systemu ---" << endl;

    // Uzywamy petli 'for' do dodania procesow
    int ids[] = { 1, 2, 3, 4 };
    int czasy[] = { 7, 3, 10, 5 }; // Laczny czas wykonania

    for (int i = 0; i < 4; ++i) {
        kolejka_procesow.dodaj_proces(ids[i], czasy[i]);
    }

    // Uruchomienie glownej petli symulacji
    symuluj_RoundRobin(kolejka_procesow, kwant_czasu);

    return 0;
}
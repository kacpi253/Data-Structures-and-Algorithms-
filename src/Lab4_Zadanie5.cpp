#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Kierunek sortowania dla kolejek priorytetowych
enum SortDirection { ASC, DESC };
// Kierunek ruchu windy
enum ElevatorDirection { UP, DOWN, IDLE };

// Wêze³ listy jednokierunkowej
struct RequestNode {
    int floor;
    RequestNode* next;
    RequestNode(int f) : floor(f), next(nullptr) {}
};

// --- Klasa kolejki wezwan ---
class RequestQueue {
private:
    RequestNode* front;
    SortDirection direction; // ASC = rosn¹co, DESC = malej¹co

public:
    RequestQueue(SortDirection dir) : front(nullptr), direction(dir) {}

    ~RequestQueue() {
        while (!is_empty()) {
            obsluz_nastepne();
        }
    }

    bool is_empty() {
        return front == nullptr;
    }

    void dodaj_wezwanie(int floor, int current_floor) {
        RequestNode* new_node = new RequestNode(floor);

        // Jeœli winda jedzie w GÓRÊ, a wezwanie jest poni¿ej aktualnego piêtra,
        // to dodaj wezwanie na koniec kolejki.
        if (direction == ASC && floor < current_floor) {
            RequestNode* current = front;
            if (current == nullptr) {
                front = new_node;
            }
            else {
                while (current->next != nullptr) current = current->next;
                current->next = new_node;
            }
            return;
        }

        // Jeœli winda jedzie w DÓ£, a wezwanie jest powy¿ej aktualnego piêtra,
        // to równie¿ dodaj wezwanie na koniec kolejki.
        if (direction == DESC && floor > current_floor) {
            RequestNode* current = front;
            if (current == nullptr) {
                front = new_node;
            }
            else {
                while (current->next != nullptr) current = current->next;
                current->next = new_node;
            }
            return;
        }

        // --- STANDARDOWE DODAWANIE POSORTOWANE ---
        bool insert_at_front = false;
        if (is_empty()) {
            insert_at_front = true;
        }
        else if (direction == ASC && new_node->floor < front->floor) {
            insert_at_front = true;
        }
        else if (direction == DESC && new_node->floor > front->floor) {
            insert_at_front = true;
        }

        if (insert_at_front) {
            new_node->next = front;
            front = new_node;
        }
        else {
            RequestNode* current = front;
            while (current->next != nullptr) {
                if (direction == ASC && current->next->floor > new_node->floor) break;
                if (direction == DESC && current->next->floor < new_node->floor) break;
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }

    int peek_next_floor() {
        if (is_empty()) return -1;
        return front->floor;
    }

    int obsluz_nastepne() {
        if (is_empty()) return -1;
        RequestNode* served_node = front;
        int floor = served_node->floor;
        front = front->next;
        delete served_node;
        return floor;
    }

    void wyswietl_stan(const string& name) {
        cout << "   " << name << ": ";
        if (is_empty()) {
            cout << "(pusta)" << endl;
            return;
        }
        RequestNode* current = front;
        while (current != nullptr) {
            cout << current->floor << " ";
            current = current->next;
        }
        cout << endl;
    }
};

// --- Klasa windy ---
class Elevator {
private:
    int current_floor;
    ElevatorDirection direction;
    int max_floor;
    int min_floor;

    RequestQueue requests_up;
    RequestQueue requests_down;

public:
    Elevator(int min_f, int max_f)
        : current_floor(min_f),
        direction(IDLE),
        max_floor(max_f),
        min_floor(min_f),
        requests_up(ASC),
        requests_down(DESC)
    {
        cout << "Winda uruchomiona (Pietra: " << min_f << " - " << max_f << ")" << endl;
    }

    void wezwij_winde(int from_floor, ElevatorDirection requested_direction) {
        if (requested_direction == UP) {
            cout << "[WEZWANIE] W GORE z pietra " << from_floor << endl;
            requests_up.dodaj_wezwanie(from_floor, current_floor);
        }
        else if (requested_direction == DOWN) {
            cout << "[WEZWANIE] W DOL z pietra " << from_floor << endl;
            requests_down.dodaj_wezwanie(from_floor, current_floor);
        }
    }

    bool czy_wszystkie_kolejki_puste() {
        return requests_up.is_empty() && requests_down.is_empty();
    }

    ElevatorDirection get_kierunek() {
        return direction;
    }

    void symuluj_krok() {
        if (direction == IDLE) {
            if (!requests_up.is_empty()) direction = UP;
            else if (!requests_down.is_empty()) direction = DOWN;
            else return;
        }

        if (direction == UP) {
            while (requests_up.peek_next_floor() == current_floor) {
                cout << "[>>] Winda (GORA) zatrzymuje sie na " << current_floor << endl;
                requests_up.obsluz_nastepne();
                this_thread::sleep_for(chrono::milliseconds(300));
            }

            if (current_floor < max_floor) current_floor++;
            if (current_floor == max_floor) {
                cout << "[!!] Winda dotarla na GORE budynku. Zmiana kierunku." << endl;
                direction = DOWN;
            }
        }

        else if (direction == DOWN) {
            while (requests_down.peek_next_floor() == current_floor) {
                cout << "[<<] Winda (DOL) zatrzymuje sie na " << current_floor << endl;
                requests_down.obsluz_nastepne();
                this_thread::sleep_for(chrono::milliseconds(300));
            }

            if (current_floor > min_floor) current_floor--;
            if (current_floor == min_floor) {
                cout << "[!!] Winda dotarla na DOL budynku. Zmiana kierunku." << endl;
                direction = UP;
                if (requests_up.is_empty()) {
                    direction = IDLE;
                    if (!requests_down.is_empty()) direction = UP;
                }
            }
        }
    }

    void wyswietl_stan_windy() {
        cout << "\n--- STAN ---" << endl;
        string dir_str = (direction == UP) ? "GORA" : (direction == DOWN) ? "DOL" : "IDLE";
        cout << "Pietro: " << current_floor << " | Kierunek: " << dir_str << endl;
        requests_up.wyswietl_stan("Kolejka GORA");
        requests_down.wyswietl_stan("Kolejka DOL");
        cout << "------------" << endl;
    }
};

// --- PRZYKLADOWE DZIALANIE ---
int main() {
    Elevator winda(0, 10);
    winda.wyswietl_stan_windy();

    // Dodajemy wezwania
    winda.wezwij_winde(3, UP);
    winda.wezwij_winde(1, UP);
    winda.wezwij_winde(7, UP);

    winda.wezwij_winde(8, DOWN);
    winda.wezwij_winde(5, DOWN);

    for (int i = 0; i < 40; ++i) {
        if (i == 15) {
            cout << "\n*** NOWE WEZWANIE W TRAKCIE ***" << endl;
            winda.wezwij_winde(9, DOWN);
        }

        winda.symuluj_krok();
        winda.wyswietl_stan_windy();

        if (winda.czy_wszystkie_kolejki_puste() && winda.get_kierunek() == IDLE) {
            cout << "\n--- Winda zakonczyla wszystkie zadania ---" << endl;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}

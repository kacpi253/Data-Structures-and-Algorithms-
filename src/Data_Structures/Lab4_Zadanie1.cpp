
#include <iostream>
using namespace std;

struct Node {
    int client_id;
    Node* next;
    Node(int id) : client_id(id), next(nullptr) {}
};

class FifoQueue {
private:
    Node* front; // WskaŸnik na pocz¹tek kolejki (do obs³ugi)
    Node* rear;  // WskaŸnik na koniec kolejki (do dodawania)

public:
    FifoQueue() : front(nullptr), rear(nullptr) {
        cout << "Utworzono pusta kolejkê sklepowa." << endl;
    }

    ~FifoQueue() {
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
    void dodaj_klienta(int client_id) {
        Node* new_node = new Node(client_id); // Alokacja pamiêci

        // Jeœli kolejka jest pusta, nowy wêze³ jest pocz¹tkiem i koñcem
        if (is_empty()) {
            front = new_node;
            rear = new_node;
        }
        else {
            // W przeciwnym razie, dodajemy na koniec
            rear->next = new_node;
            rear = new_node;
        }
        cout << "[+] Klient " << client_id << " dolaczyl do kolejki." << endl;
    }

    // Obs³uguje klienta, usuwaj¹c go z pocz¹tku kolejki (operacja dequeue)     verbose aby wyciszyæ komuniakty
    void obsluz_klienta(bool verbose = true) {
        if (is_empty()) {
            if (verbose) {
                cout << "[-] Kolejka jest pusta. Brak klientow do obslugi." << endl;
            }
            return;
        }
        Node* served_node = front;
        int client_id = served_node->client_id;
        front = front->next;

        // Jeœli po usuniêciu kolejka sta³a siê pusta, musimy te¿ zaktualizowaæ 'rear'
        if (front == nullptr) {
            rear = nullptr;
        }

        // Zwalniamy pamiêæ po obs³u¿onym wêŸle
        delete served_node;

        if (verbose) {
            cout << "[->] Klient " << client_id << " zostal obsluzony." << endl;
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

        cout << "\n=== STAN KOLEJKI ===" << endl;
        cout << "   (Poczatek)" << endl;
        Node* current = front;
        while (current != nullptr) {
            cout << "      Klient " << current->client_id << endl;
            current = current->next;
        }
        cout << "   (Koniec)" << endl;
        cout << "====================" << endl;
    }
};


using namespace std;
int main()
{
    FifoQueue kolejka;

    cout << "Rozpoczynamy symulacje sklepu" << endl;
    cout << "Do sklepu wchodzi 4 klientow" << endl;
    for (int i = 0; i < 4; i++) {
        kolejka.dodaj_klienta(100 + i);
    }
    kolejka.wyswietl_stan();
    cout << "Ze sklepu wychodzi 2 klientow" << endl;
    for (int i = 0; i < 2; i++) {
        kolejka.obsluz_klienta();
    }
    kolejka.wyswietl_stan();


}


#include <iostream>
#include <string>

using namespace std;


// Pojedynczy element stosu
struct Element {
    char znak;
    Element* nastepny;

    Element(char c) : znak(c), nastepny(nullptr) {}
};

class Stos {
private:
    Element* wierzch;
public:
    Stos() : wierzch(nullptr) {}

    void push(char c) {
        Element* nowy = new Element(c);         //dodanie znaku na szczyt
        nowy->nastepny = wierzch;
        wierzch = nowy;
    }


    void pop() {
        if (wierzch != nullptr) {
            Element* doUsuniecia = wierzch;         //zdjecie znaku ze szczytu i usuniecie
            wierzch = wierzch->nastepny;
            delete doUsuniecia;
        }
    }
    char top() {
        if (wierzch == nullptr) return 0;           // Sprawdzenie co jest na górze
        return wierzch->znak;
    }

    bool czyPusty() {
        return wierzch == nullptr; 
    }
    ~Stos() {
        while (!czyPusty()) {
            pop();
        }
    }
};

char dajDobryZamykajacy(char otwierajacy) {
	switch (otwierajacy) {
	case '(': return ')';
	case '{': return '}';
	case '[': return ']';
	default: return '\0'; 
	}
}

bool czyOtwierajacy(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool czyZamykajacy(char c) {
    return c == ')' || c == ']' || c == '}';
}
            
void validate(string s) {
    Stos stos;

    for (int i = 0; i < s.length(); i++) {
        char obecny = s[i];
        if (czyOtwierajacy(obecny)) {
            stos.push(obecny);

        }
        else if (czyZamykajacy(obecny)) {

            if (stos.czyPusty()) {
                cout << "BLAD  " << i << ": niespodziewany zamykajacy " << obecny << endl;  //b³¹d 1. zamkniety nawias przy pustym stosie
                return;
            }
            char ostatniOtwarty = stos.top();
            char oczekiwany = dajDobryZamykajacy(ostatniOtwarty);

            if (obecny != oczekiwany) {
                cout << "BLAD  " << i << ": oczekiwano " << oczekiwany             //b³¹d 2. Nawias zamykaj¹cy nei pasuje do otwartego
                    << ", otrzymano " << obecny << endl;
                return;
            }

            stos.pop();
        }

    }

    if (!stos.czyPusty()) {
        char brakujacy = dajDobryZamykajacy(stos.top());
        cout << "BLAD: koniec wejscia, brak zamkniecia dla " << brakujacy << endl;          //b³¹d 3, Brak zamkniecia na koncu
    }
    else {
        cout << "OK." << endl;
    }
}

int main() {
    // Przyk³ady testowe z zadania
    cout << "Test 1: a*(b+[c-d])" << endl;
    validate("a*(b+[c-d])");
    cout << endl;

    cout << "Test 2: ( [ ] }" << endl;
    validate("( [ ] }");
    cout << endl;

    cout << "Test 3: ([a+b]" << endl;
    validate("([a+b]");
    cout << endl;

    cout << "Test 4: ())" << endl;
    validate("())");
    cout << endl;

    return 0;
}


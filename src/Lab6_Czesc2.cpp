#include <iostream>
using namespace std;

struct BookData {
    string author;
    int copies;
    int year;
};

class BookNode
{
public:

    string key;
    BookData data;

    BookNode* Left;
    BookNode* Right;
    BookNode* Parent;

    BookNode(const string& title, const string& author, int copies, int year,
        BookNode* p = nullptr, BookNode* l = nullptr, BookNode* r = nullptr)
        : key(title), data({ author, copies, year }), Parent(p), Left(l), Right(r) {}

    ~BookNode() = default;
};

class BookCatalogue{
private:
	BookNode* root;
    
    static BookNode* findNode(BookNode* root, const string& title) {
        BookNode* current = root;
        while (current != nullptr) {
            if (title == current->key) {
                return current;
            }
            else if (title < current->key) {
                current = current->Left;
            }
            else {
                current = current->Right;
            }
        }
        return nullptr; // Nie znaleziono
    }

    static BookNode* min(BookNode* root) {
        if (root == nullptr) return nullptr;
        BookNode* current = root;
        while (current->Left != nullptr) {
            current = current->Left;
        }
        return current;
    }
	static BookNode* max(BookNode* root) {
		if (root == nullptr) return nullptr;
		BookNode* current = root;
		while (current->Right != nullptr) {
			current = current->Right;
		}
		return current;
	}

	static BookNode* inOrderSuccessor(BookNode* n) {
		if (n == nullptr) return nullptr;
		if (n->Right != nullptr) {
			return min(n->Right);
		}
		BookNode* p = n->Parent;
		while (p != nullptr && n == p->Right) {
			n = p;
			p = p->Parent;
		}
		return p;
	}

	void transplant(BookNode* u, BookNode* v) {
		if (u->Parent == nullptr) {
			root = v;
		}
		else if (u == u->Parent->Left) {
			u->Parent->Left = v;
		}
		else {
			u->Parent->Right = v;
		}
		if (v != nullptr) {
			v->Parent = u->Parent;
		}
	}

	static void deleteTree(BookNode* node) {
		if (node == nullptr) return;
		deleteTree(node->Left);
		deleteTree(node->Right);
		delete node;
	}

public:
	BookCatalogue() : root(nullptr) {}
	~BookCatalogue() {
		deleteTree(root);
	}

	// 1.  Dodawanie ksi¹¿ki do katalogu 
    void addBook(const string& title, const string& author, int copies, int year) {
        BookNode* newNode = new BookNode(title, author, copies, year);

        if (root == nullptr) {
            root = newNode;
            cout << "Pomyslnie dodano now¹ ksiazkê: " << title << endl;
            return;
        }

        BookNode* current = root;
        BookNode* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (title < current->key) {
                current = current->Left;
            }
            else if (title > current->key) {
                current = current->Right;
            }
            else {
                current->data.copies += copies;
                current->data.author = author;
                current->data.year = year;

                delete newNode; // Zwalniamy nieu¿ywany wêze³
                cout << "Ksiazka \"" << title << "\" ju¿ istnieje. Zwiekszono liczbe egzemplarzy o " << copies << ". Nowa liczba: " << current->data.copies << endl;
                return;
            }
        }
        newNode->Parent = parent;
        if (title < parent->key) {
            parent->Left = newNode;
        }
        else {
            parent->Right = newNode;
        }
        cout << "Pomyslnie dodano nowa ksiazke: " << title << endl;
    }


	//2. Usuwanie ksi¹¿ki z katalogu
    void removeBook(const string& title) {
        BookNode* z = findNode(this->root, title);
        if (z == nullptr) {
            cout << "B³¹d: Ksiazka \"" << title << "\" nie zostala znaleziona." << endl;
            return;
        }

        if (z->data.copies > 1) {
            z->data.copies--;
            cout << "Zmniejszono liczbê egzemplarzy ksiazki \"" << title << "\". Pozostalo: " << z->data.copies << endl;
        }
        else {
            BookNode* w = z; 
            if (z->Left == nullptr || z->Right == nullptr) {
                w = z; 
            }
            else {
                w = inOrderSuccessor(z); 
            }

            BookNode* y = (w->Left != nullptr) ? w->Left : w->Right; 

            transplant(w, y); 

            if (w != z) {
                z->key = w->key;
                z->data = w->data;
            }
            cout << "Pomyslnie usunieto ostatni egzemplarz ksiazki \"" << title << "\" (wêzel usunity: " << w->key << ")." << endl;
            delete w;
        }
    }

    // --- 3. Wyszukiwanie ksi¹¿ki po tytule (Search) ---
    void searchBook(const string& title) {
        BookNode* node = findNode(this->root, title);
        if (node != nullptr) {
            cout << "\nZnaleziono ksiazkê!" << endl;
            
            cout << "Tytul: " << node->key << endl;
            cout << "Autor: " << node->data.author << endl;
            cout << "Rok wydania: " << node->data.year << endl;
            cout << "Liczba egzemplarzy: " << node->data.copies << endl;
        }
        else {
            cout << "\nKomunikat: Ksiazka \"" << title << "\" nie zosta³a znaleziona w katalogu." << endl;
        }
    }


    // --- 4. Wyœwietlanie ca³ego katalogu w kolejnoœci alfabetycznej (In-Order Traversal) ---
    static void printBook(BookNode* node) {
        if (node == nullptr) return;
        printBook(node->Left);

        cout << "- Tytul: " << node->key
            << " | Autor: " << node->data.author
            << " | Egz.: " << node->data.copies
            << " | Rok: " << node->data.year << endl;

        printBook(node->Right);
    }

    void displayCatalogueInOrder() {
        cout << "\n--- CALY KATALOG KSIAZEK (Alfabetycznie) ---" << endl;
        if (root == nullptr) {
            cout << "Katalog jest pusty." << endl;
            return;
        }
        printBook(root);
        cout << "------------------------------------------" << endl;
    }

    void findMinTitle() {
        BookNode* node = min(this->root);
        if (node != nullptr) {
            cout << "\nKsiazka z najwczesniejszym tytulem (alfabetycznie):" << endl;
            cout << "Tytul: " << node->key << " | Autor: " << node->data.author << " | Egz.: " << node->data.copies << endl;
        }
        else {
            cout << "\nKatalog jest pusty." << endl;
        }
    }

    void findMaxTitle() {
        BookNode* node = max(this->root);
        if (node != nullptr) {
            cout << "\nKsiazka z najpozniejszym tytulem (alfabetycznie):" << endl;
            cout << "Tytul: " << node->key << " | Autor: " << node->data.author << " | Egz.: " << node->data.copies << endl;
        }
        else {
            cout << "\nKatalog jest pusty." << endl;
        }
    }

    // --- 6. Wyœwietlanie ksi¹¿ek wydanych w okreœlonym zakresie lat ---
    void displayBooksByYearRange(BookNode* node, int startYear, int endYear) {
        if (node == nullptr) return;
        displayBooksByYearRange(node->Left, startYear, endYear);

        // Filtracja
        if (node->data.year >= startYear && node->data.year <= endYear) {
            cout << "- Tytul: " << node->key
                << " | Autor: " << node->data.author
                << " | Egz.: " << node->data.copies
                << " | Rok: " << node->data.year << endl;
        }

        displayBooksByYearRange(node->Right, startYear, endYear);
    }

    void displayBooksInYearRange(int startYear, int endYear) {
        cout << "\n--- KSIAZKI WYDANE W LATACH [" << startYear << " - " << endYear << "] ---" << endl;
        if (root == nullptr) {
            cout << "Katalog jest pusty." << endl;
            return;
        }
        displayBooksByYearRange(root, startYear, endYear);
        cout << "---------------------------------------------------------" << endl;
    }


};

int main() {

    BookCatalogue catalogue;

    // --- 1. Dodawanie ksiazek ---
    cout << "--- DODAWANIE KSIAZEK ---" << endl;
    catalogue.addBook("Wladca Pierscieni", "J.R.R. Tolkien", 5, 1954);
    catalogue.addBook("Zbrodnia i Kara", "Fiodor Dostojewski", 2, 1866);
    catalogue.addBook("1984", "George Orwell", 10, 1949);
    catalogue.addBook("Maja", "Pszczolka Maja", 3, 1912);
    catalogue.addBook("Dziady", "Adam Mickiewicz", 1, 1823);
    catalogue.addBook("Maja", "Pszczolka Maja", 2, 1912); // Zwiekszenie egzemplarzy
    cout << "-------------------------" << endl;

    // --- 4. Wyswietlanie calego katalogu w kolejnosci alfabetycznej ---
    catalogue.displayCatalogueInOrder();

    // --- 3. Wyszukiwanie ksiazki po tytule ---
    catalogue.searchBook("1984");
    catalogue.searchBook("Nieistniejaca Ksiazka");

    // --- 5. Znajdowanie najwczesniejszego i najpozniejszego tytulu ---
    catalogue.findMinTitle();
    catalogue.findMaxTitle();

    // --- 6. Wyswietlanie ksiazek wydanych w okreslonym zakresie lat ---
    catalogue.displayBooksInYearRange(1900, 1950);

    // --- 2. Usuwanie ksiazki z katalogu ---
    cout << "\n--- USUWANIE KSIAZEK ---" << endl;

    // Test: Zmniejszenie liczby egzemplarzy (Wladca Pierscieni ma 5)
    catalogue.removeBook("Wladca Pierscieni");
    catalogue.searchBook("Wladca Pierscieni");

    // Test: Usuniecie wezla (Dziady ma 1 egzemplarz)
    catalogue.removeBook("Dziady");
    catalogue.searchBook("Dziady");

    // Test: Usuniecie wezla z 2 dziecmi (1984 ma 10 egzemplarzy, usuwamy po 10. razie)
    for (int i = 0; i < 9; ++i) { // Zmniejsz do 1 egzemplarza
        catalogue.removeBook("1984");
    }
    catalogue.removeBook("1984"); // Usuniecie wezla
    catalogue.searchBook("1984");

    cout << "-------------------------" << endl;

    // --- 4. Wyswietlanie po usunieciu ---
    catalogue.displayCatalogueInOrder();

    return 0;

}

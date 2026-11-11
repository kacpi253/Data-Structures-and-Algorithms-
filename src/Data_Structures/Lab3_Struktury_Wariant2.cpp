
#include <iostream>
using namespace std;


struct Node {
    int data;
    Node* next;

};

class Stack {
private:
    Node* top;
public:
    Stack() {
        top = nullptr;
    }
    ~Stack() {
        while (!isEmpty()) {
            pop(); 
        }
    }

    void push(int value) {
       
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = top;
        top = newNode;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stos jest pusty!" << endl;
            return INT_MIN;
        }
        else {
            int value = top->data;
            Node* temp = top;
            top = top->next;
            delete temp;
            return value;
        }
    }

    int vTop() {
        if (isEmpty()) {
            cout << "Stos jest pusty!" << endl;
            return INT_MIN;
        }
        else {
            return top->data;
        }
    }

    // Funkcja isFull() nie jest ju¿ potrzebna w implementacji dynamicznej

    // Sprawdza, czy stos jest pusty
    bool isEmpty() {
        return top == nullptr;
    }



};
int main()
{
    std::cout << "Hello World!\n";
}


///   ZAD3 bez pomocniczego stosu ///

// Specjalista: Wk³ada 'item' na dno stosu 's'.
void insertAtBottom(Stack& s, int item) {
    if (s.isEmpty()) {
        s.push(item); // Jeœli stos pusty, po prostu dodaj.
        return;
    }
    // Jeœli nie jest pusty:
    int temp = s.pop();      // Zdejmij górny element i zapamiêtaj.
    insertAtBottom(s, item); // Zleæ w³o¿enie 'item' do mniejszego stosu.
    s.push(temp);            // Gdy 'item' jest ju¿ na dnie, od³ó¿ zapamiêtany element.
}

// G³ówny szef: Odwraca ca³y stos 's'.
void reverse(Stack& s) {
    if (s.isEmpty()) {
        return; // Pustego stosu nie trzeba odwracaæ.
    }
    // Jeœli nie jest pusty:
    int item = s.pop(); // Zdejmij górny element i zapamiêtaj.
    reverse(s);         // Zleæ odwrócenie reszty stosu.
    insertAtBottom(s, item); // U¿yj specjalisty, by w³o¿yæ zapamiêtany element na dno.
}


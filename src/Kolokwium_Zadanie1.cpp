#include <iostream>
#include <string>
#include <vector>


/**
 * Zadanie:
 * Napisz program, który posortuje (w dowolny sposób), a następnie odwróci listę
 * jednokierunkową w segmentach o długości k. Jeśli ostatni segment ma mniej niż
 * k elementów, nie należy go odwracać. Funkcja powinna działać w miejscu, czyli
 * bez tworzenia nowej listy.
 */

// Struktura węzła listy jednokierunkowej
struct Node {
  int data;
  Node *next;

  Node(int val) : data(val), next(nullptr) {}
};

// =============================================================
// Narzędzia pomocnicze (Helper functions)
// =============================================================

// Funkcja do wypisywania zawartości listy na standardowe wyjście
void printList(Node *head, const std::string &prefix = "") {
  std::cout << prefix;
  Node *temp = head;
  while (temp) {
    std::cout << temp->data;
    if (temp->next)
      std::cout << " -> ";
    temp = temp->next;
  }
  std::cout << std::endl;
}

// Funkcja do tworzenia listy z wektora (ułatwia inicjalizację danych testowych)
Node *createList(const std::vector<int> &values) {
  if (values.empty())
    return nullptr;
  Node *head = new Node(values[0]);
  Node *current = head;
  for (size_t i = 1; i < values.size(); ++i) {
    current->next = new Node(values[i]);
    current = current->next;
  }
  return head;
}

// Funkcja do zwalniania pamięci całej listy
void deleteList(Node *head) {
  while (head) {
    Node *temp = head;
    head = head->next;
    delete temp;
  }
}

// =============================================================
// Część 1: Sortowanie (Merge Sort)
// Wybrano sortowanie przez scalanie, ponieważ jest to efektywny algorytm dla
// list, działający w czasie O(N log N) i niewymagający losowego dostępu do
// elementów.
// =============================================================

// Dzieli listę na dwie połowy przy użyciu metody "wolny i szybki wskaźnik"
void splitList(Node *source, Node **frontRef, Node **backRef) {
  Node *fast;
  Node *slow;

  if (source == nullptr || source->next == nullptr) {
    *frontRef = source;
    *backRef = nullptr;
  } else {
    slow = source;
    fast = source->next; // Szybki startuje krok dalej

    while (fast != nullptr) {
      fast = fast->next;
      if (fast != nullptr) {
        slow = slow->next;
        fast = fast->next;
      }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr; // Rozcięcie listy
  }
}

// Scala dwie posortowane listy w jedną
Node *sortedMerge(Node *a, Node *b) {
  if (a == nullptr)
    return b;
  if (b == nullptr)
    return a;

  Node *result = nullptr;

  if (a->data <= b->data) {
    result = a;
    result->next = sortedMerge(a->next, b);
  } else {
    result = b;
    result->next = sortedMerge(a, b->next);
  }
  return result;
}

// Główna funkcja sortująca
void mergeSort(Node **headRef) {
  Node *head = *headRef;
  Node *a;
  Node *b;

  // Przypadek bazowy: lista pusta lub jednoelementowa
  if ((head == nullptr) || (head->next == nullptr)) {
    return;
  }

  // Podział listy na dwie podlisty a i b
  splitList(head, &a, &b);

  // Rekurencyjne sortowanie podlist
  mergeSort(&a);
  mergeSort(&b);

  // Scalenie posortowanych list
  *headRef = sortedMerge(a, b);
}

// =============================================================
// Część 2: Odwracanie segmentów o długości k (Reverse Nodes in k-Group)
// Algorytm działa w miejscu (in-place) i ma złożoność O(N).
// =============================================================

Node *reverseKGroup(Node *head, int k) {
  if (!head || k <= 1)
    return head;

  // Używamy węzła "dummy" na stosie, aby uprościć operacje na początku listy
  // (tzw. sentinel node)
  Node dummy(0);
  dummy.next = head;

  Node *prevGroupEnd =
      &dummy;        // Koniec poprzedniego segmentu (początkowo dummy)
  Node *curr = head; // Początek aktualnego segmentu do przetworzenia

  while (curr) {
    // Krok 1: Sprawdź, czy pozostało wystarczająco dużo elementów (k)
    Node *check = curr;
    int count = 0;
    while (check && count < k) {
      check = check->next;
      count++;
    }

    // Jeśli segment jest krótszy niż k, zostawiamy go bez zmian i kończymy
    if (count < k)
      break;

    // Krok 2: Odwróć segment k elementów
    Node *groupPrev = nullptr;
    Node *groupCurr = curr;
    Node *groupNext = nullptr;

    for (int i = 0; i < k; i++) {
      groupNext = groupCurr->next;
      groupCurr->next = groupPrev; // Odwrócenie wskaźnika
      groupPrev = groupCurr;
      groupCurr = groupNext;
    }

    // Po pętli:
    // groupPrev wskazuje na nową głowę tego segmentu (były k-ty element)
    // curr wskazuje na nowy ogon tego segmentu (były 1-szy element)
    // groupCurr wskazuje na początek następnego segmentu (k+1 element)

    // Krok 3: Połącz odwrócony segment z resztą listy
    prevGroupEnd->next =
        groupPrev;          // Poprzednia część -> Nowy początek segmentu
    curr->next = groupCurr; // Nowy koniec segmentu -> Następna część listy

    // Krok 4: Przesuń wskaźniki do następnej iteracji
    prevGroupEnd =
        curr; // Obecny ogon staje się poprzednim ogonem dla następnej pętli
    curr = groupCurr; // Przejdź do następnego segmentu
  }

  return dummy.next;
}

// =============================================================
// Funkcja Main
// =============================================================

int main() {
  // 1. Inicjalizacja danych (Przykład z zadania)
  // Lista: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
  std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};
  int k = 3;

  Node *listHead = createList(data);

  std::cout << "=== Zadanie Projektowe ===" << std::endl;
  printList(listHead, "Lista wejściowa: ");
  std::cout << "k = " << k << std::endl << std::endl;

  // 2. Sortowanie
  // (Dla danych z przykładu lista jest już posortowana, ale wywołujemy funkcję
  // zgodnie z treścią zadania)
  mergeSort(&listHead);
  // Opcjonalne: gdyby dane nie były posortowane, tutaj byśmy to zobaczyli
  // printList(listHead, "Lista posortowana: ");

  // 3. Odwracanie w segmentach
  listHead = reverseKGroup(listHead, k);

  // 4. Wynik
  printList(listHead, "Lista po odwróceniu segmentów: ");

  // Sprzątanie pamięci
  deleteList(listHead);

  return 0;
}

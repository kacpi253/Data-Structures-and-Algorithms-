#include <iostream>
using namespace std;

class BSTNode
{
public:
    BSTNode(int k, BSTNode* p = nullptr, BSTNode* l = nullptr, BSTNode* r = nullptr)
        : key(k), Parent(p), Left(l), Right(r) {}

    int key;
    BSTNode* Left;
    BSTNode* Right;
    BSTNode* Parent;

    static void printTreePreOrder(BSTNode* node) {
		if (node == nullptr) return;
		cout << node->key << " ";
		printTreePreOrder(node->Left);
		printTreePreOrder(node->Right);
    }

    static void printTreePostOrder(BSTNode* node) {
		if (node == nullptr) return;
		printTreePostOrder(node->Left);
		printTreePostOrder(node->Right);
		cout << node->key << " ";
    }
    static void printTreeInOrder(BSTNode* node) {
        if (node == nullptr) return;
        printTreeInOrder(node->Left);
        cout << node->key << " ";
        printTreeInOrder(node->Right);
    }

    static BSTNode* min(BSTNode* root) {
        if (root == nullptr) return nullptr;
        BSTNode* current = root;
        while (current->Left != nullptr) {
            current = current->Left;
        }
        return current;
    }

    static BSTNode* max(BSTNode* root) {
		if (root == nullptr) return nullptr;
		BSTNode* current = root;
        while (current->Right != nullptr) {
            current = current->Right;
        }
		return current;
    }

    static int calculateHeight(BSTNode* node) {
		if (node == nullptr) return 0;
		int leftHeight = calculateHeight(node->Left);
		int rightHeight = calculateHeight(node->Right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    static BSTNode* inOrderSuccessor(BSTNode* n) {
        if (n == nullptr) return nullptr;
        if (n->Right != nullptr) {
            return min(n->Right);
        }
        BSTNode* p = n->Parent;
        BSTNode* current = n;
        while (p != nullptr && current == p->Right) {
            current = p;
            p = p->Parent;
        }
        return p; 
    }
    static BSTNode* inOrderPredecessor(BSTNode* n) {
        if (n == nullptr) return nullptr;
        if (n->Left != nullptr) {
            return max(n->Left);
        }
        BSTNode* p = n->Parent;
        BSTNode* current = n;
        while (p != nullptr && current == p->Left) {
            current = p;
            p = p->Parent;
        }
        return p; 
    }
    static BSTNode* Insert(BSTNode* root, int key) {
        // Tworzymy nowy wêze³
        BSTNode* newNode = new BSTNode(key);

        // Przypadek 1: Drzewo jest puste
        if (root == nullptr) {
            return newNode; // Nowy wêze³ jest teraz korzeniem
        }

        // Przypadek 2: Drzewo nie jest puste
        BSTNode* current = root;
        BSTNode* parent = nullptr;

        while (current != nullptr) {
            parent = current; // Zapisujemy rodzica
            if (key < current->key) {
                current = current->Left;
            }
            else if (key > current->key) {
                current = current->Right;
            }
            else {
                // Klucz ju¿ istnieje, nie robimy nic (i zwalniamy pamiêæ)
                delete newNode;
                return root; // Zwracamy oryginalny korzeñ
            }
        }

        // ZnaleŸliœmy miejsce. 'parent' to liœæ, do którego do³¹czamy.
        newNode->Parent = parent;
        if (key < parent->key) {
            parent->Left = newNode;
        }
        else {
            parent->Right = newNode;
        }

        return root; // Zwracamy oryginalny korzeñ
    }

    /**
     * @brief Pomocnicza funkcja do znajdowania wêz³a o danym kluczu.
     */
    static BSTNode* findNode(BSTNode* root, int key) {
        BSTNode* current = root;
        while (current != nullptr) {
            if (key == current->key) {
                return current;
            }
            else if (key < current->key) {
                current = current->Left;
            }
            else {
                current = current->Right;
            }
        }
        return nullptr; // Nie znaleziono
    }

    static BSTNode* Delete(BSTNode* root, int key) {
        // Krok 1: ZnajdŸ wêze³ do usuniêcia
        BSTNode* z = findNode(root, key);
        if (z == nullptr) {
            return root; // Nie znaleziono klucza, zwracamy oryginalny korzeñ
        }
        BSTNode* w;
        if (z->Left == nullptr || z->Right == nullptr) {
            w = z;
        }
        else {
            w = inOrderSuccessor(z); // Nastêpnik w jest minimum w prawym poddrzewie z
        }

        // y to jedyne dziecko wêz³a w (mo¿e byæ nullptr)
        BSTNode* y = (w->Left != nullptr) ? w->Left : w->Right;
        if (y != nullptr) {
            y->Parent = w->Parent; // Ustaw rodzica dla dziecka
        }

        // £¹czenie wêz³a y z rodzicem wêz³a w
        if (w->Parent == nullptr) {
            root = y; // Wêze³ w by³ korzeniem, y jest nowym korzeniem
        }
        else if (w == w->Parent->Left) {
            w->Parent->Left = y; // Wêze³ w by³ lewym dzieckiem
        }
        else {
            w->Parent->Right = y; // Wêze³ w by³ prawym dzieckiem
        }
        if (w != z) {
            z->key = w->key;
        }

        delete w;

        return root;
    }

    
};

int main() {


    
    BSTNode* root = nullptr;
    root = BSTNode::Insert(root, 50);
    root = BSTNode::Insert(root, 30);
    root = BSTNode::Insert(root, 70);
    root = BSTNode::Insert(root, 20);
    root = BSTNode::Insert(root, 40);
    root = BSTNode::Insert(root, 60);
    root = BSTNode::Insert(root, 80);



    // Wypisanie przed usuniêciem
    cout << "InOrder przed usuniêciem: ";
    BSTNode::printTreeInOrder(root); // Oczekiwane: 20 30 40 50 60 70 80 
    cout << endl;

    // Usuñ wêze³ 50 (korzeñ z dwoma dzieæmi)
    root = BSTNode::Delete(root, 50);

    cout << "InOrder po usuniêciu 50: ";
    BSTNode::printTreeInOrder(root); // Oczekiwane: 20 30 40 60 70 80 (70 jako nowy korzeñ)
    cout << endl;

    // Usuñ wêze³ 20 (liœæ)
    root = BSTNode::Delete(root, 20);

    cout << "InOrder po usuniêciu 20: ";
    BSTNode::printTreeInOrder(root);
    cout << endl;
    return 0;

}

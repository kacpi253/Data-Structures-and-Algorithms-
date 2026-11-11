
#include <iostream>
#include <string>


#define MAX_SIZE 100
#define INT_MIN -2147483648

using namespace std;

// do zamiany na stos dynamiczny

struct charNode {
    char data;
    charNode* next;

};

struct Node {
    int data;
    Node* next;

};
class charStack {
private:
    charNode* top;
public:

    charStack() {
        top = nullptr;
    }
    ~charStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(char value) {
        charNode* newNode = new charNode;
        newNode->data = value;
        newNode->next = top;
        top = newNode;

    }

    char pop() {
        if (isEmpty()) { cout << "Stos jest pusty!" << endl; return '\0'; }
        else {
            charNode* temp = top;
            int poppedValue = top->data;
            top = top->next;
            delete temp;
            return poppedValue;
        }
    }

    char vTop() {
        if (isEmpty()) { cout << "Stos jest pusty!" << endl; return '\0'; }
        else {
            return top->data;
        }
    }

    bool isEmpty() {
        return top == nullptr;
    }

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
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = top;
        top = newNode;

    }

    int pop(){
        if (isEmpty()) { cout << "Stos jest pusty!" << endl; return INT_MIN; }
        else {
            Node* temp = top;
            int poppedValue = top->data;
            top = top->next;
            delete temp;
            return poppedValue; 
        }
    }

    int vTop() {
        if (isEmpty()) { cout << "Stos jest pusty!" << endl; return INT_MIN; }
        else {
            return top->data;
        }
    }

    bool isEmpty() {
        return top == nullptr;
    }
       
};

///  ZAD 2  ///
bool AreBracketsBalanced(string expr){
    Stack s;

    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        }
        else if (c == ')' || c == ']' || c == '}') {
            if (s.isEmpty()) return false;

            char temp = s.pop();

            if (temp == '(' && c == ')') {
                continue;
            }
            else if (temp == '[' && c == ']') {
                continue;
            }
            else if (temp == '{' && c == '}') {
                continue;
            }
            else return false;
        }
    }
    return s.isEmpty();

}

///  ZAD 3  ///
Stack reverseStack(Stack s) {
    Stack r;
    while (!s.isEmpty()) {
        r.push(s.pop());
    }
    return r;
}


///   ZAD4   ///

void DecToBin(int d) {
    Stack t;
    while (d != 0) {
        t.push(d % 2);
        d = d / 2;

    }
    while (!t.isEmpty()) {
        cout << t.pop();
    }
    cout << endl;
}


//    ZAD 5     ///

int procedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;

}

int applyOp(int a, int b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    }

}


int Evaluate(string expr) {
    Stack values;
    charStack ops;

    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') {
            continue;
        }
        else if (isdigit(expr[i])) {
            int val = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                val = (val * 10) + (expr[i] - '0'); //konwersja na liczbe z ascii
                i++;
            }
            values.push(val);
            i--;
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }
        else if (expr[i] == ')') {
            while (!ops.isEmpty() && ops.vTop() != '(') {
                // Zdejmij dwie wartoœci i operator
                int val2 =  values.pop();
                int val1 =  values.pop();
                char op =  ops.pop();

                // Wykonaj operacjê i wrzuæ wynik z powrotem
                values.push(applyOp(val1, val2, op));
            }
            // Usuñ nawias otwieraj¹cy ze stosu
            if (!ops.isEmpty()) {
                ops.pop();
            }
        }
        else {
            while (!ops.isEmpty() && procedence(ops.vTop()) >= procedence(expr[i])) {           // 55 + 3 * 2
                int val2 = values.pop();
                int val1 = values.pop();
                char op = ops.pop();
                values.push(applyOp(val1, val2, op));

            }
            ops.push(expr[i]);
        } 

        }
    while (!ops.isEmpty()) {
        int val2 =  values.pop();
        int val1 =  values.pop();
        char op =  ops.pop();

        values.push(applyOp(val1, val2, op));
    }
    return values.vTop();
}

int main()
{
    cout << Evaluate("3 + 5 * (4 + 6)") << endl;

    DecToBin(45);
    cout << "Podaj wyrazenie z nawiasami" << endl;
    string expr;
    getline(cin, expr);
    if (AreBracketsBalanced(expr)) {
        cout << "Nawiasy sa poprawne!!" << endl;
    }
    else {
        cout << "Nawiasy sa niepoprawne!!" << endl;
    }
}


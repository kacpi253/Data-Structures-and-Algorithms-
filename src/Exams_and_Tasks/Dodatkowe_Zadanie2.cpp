
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct President {      //Dane prezeydentów
    string fullName;
    int startYear;
    int endYear;
    string party;
};

// skopiowana funkcja split 
string* split(string s, string delimiter) {

    int pos_start = 0;
    int pos_end;
    int delim_len = delimiter.length();

    string token;
    static string buffer[10];

    int idx = 0;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {

        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;

        buffer[idx] = token;
        idx++;

         //cout << idx << endl;

    }

    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    buffer[idx] = token;

    //cout << buffer[0] << endl;

    return buffer;

}

void loadData(string path, President* presidents[], int& count) {
    string line;
    ifstream myfile(path);

    if (myfile.is_open()){
        count = 0; //wyzerowanie licznika prezydentów

        while (getline(myfile, line))
        {
           
            //split the string

            string* buffer = split(line, ";");
            President* newPresident = new President();

            // Przypisujemy dane z bufora do pól nowego obiektu
            // buffer[0] to imiê i nazwisko, buffer[1] to rok startu itd.
            newPresident->fullName = buffer[0];
            newPresident->startYear = stoi(buffer[1]); // stoi() konwertuje string na int
            newPresident->endYear = stoi(buffer[2]);
            newPresident->party = buffer[3];

            // Zapisujemy wskaŸnik do nowego obiektu w naszej tablicy
            presidents[count] = newPresident;
            count++; // Zwiêkszamy licznik prezydentów
        }
        myfile.close();
        cout << "Wczytano dane " << count << " prezydentow." << endl;
    }
    else {
        cout << "Nie mozna otworzyc pliku: " << path << endl;
    }
}

// Funkcja pomocnicza do wyœwietlania danych
void printPresidents(President* presidents[], int count) {
    cout << "\n--- Lista Prezydentow ---" << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        cout << "Prezydent: " << presidents[i]->fullName
            << ", Kadencja: " << presidents[i]->startYear << "-" << presidents[i]->endYear
            << ", Partia: " << presidents[i]->party << endl;
    }
}



//ZAD 2//
void findShortestTermPresidents(President* presidents[], int count) {
    if (count == 0) {
        cout << "Lista prezydentow jest pusta, nie mozna wykonac operacji." << endl;
        return; 
    }

    // Kadencja pierwszego prezydenta
    int shortestTerm = presidents[0]->endYear - presidents[0]->startYear;

    //reszta prezydentow
    for (int i = 1; i < count; i++) {
        int currentTerm = presidents[i]->endYear - presidents[i]->startYear;

        if (currentTerm < shortestTerm) {
            shortestTerm = currentTerm;
        }
    }

    cout << "\n--- Prezydenci o najkrotszej kadencji (" << shortestTerm << " lat) ---" << endl;
    for (int i = 0; i < count; i++) {
        int term = presidents[i]->endYear - presidents[i]->startYear; //dlugosc kadencji
        if (term == shortestTerm) {
            cout << " - " << presidents[i]->fullName
                << ", Dlugosc kadencji: " << term << " lat" << endl;
        }
    }
}

//ZAD 3 //
void findPresidentsByParty(President* presidents[], int count, string partyName) {
    cout << "\n--- Prezydenci z partii: '" << partyName << "' ---" << endl;
    int foundCount = 0; 

    // Przechodzimy pêtl¹ po wszystkich prezydentach
    for (int i = 0; i < count; i++) {
        if (presidents[i]->party == partyName) {
            cout << " - " << presidents[i]->fullName
                << " (Kadencja: " << presidents[i]->startYear << "-" << presidents[i]->endYear << ")" << endl;
            foundCount++; // Zwiêkszamy licznik znalezionych
        }
    }
    if (foundCount == 0) {
        cout << "Nie znaleziono prezydentow nalezacych do tej partii." << endl;
    }
}

//ZAD4 //
void findPresidentsByYear(President* presidents[], int count, int year){
    cout << "\n--- Prezydent urzedujacy w roku: " << year << endl;
    int foundCount = 0;

    // Przechodzimy pêtl¹ po wszystkich prezydentach
    for (int i = 0; i < count; i++) {
        if (presidents[i]->startYear <= year && presidents[i]->endYear >= year ) {
            cout << " - " << presidents[i]->fullName
                << " (Kadencja: " << presidents[i]->startYear << "-" << presidents[i]->endYear << ")" << endl;
            foundCount++; // Zwiêkszamy licznik znalezionych
        }
    }
    if (foundCount == 0) {
        cout << "Nie znaleziono prezydentow urzedujacym w tym roku" << endl;
    }
}



int main()
{
    // sta³y rozmiar tablicy z iloœci¹ prezydentów
    const int MAX_PRESIDENTS = 25;

   // Tablica wskaŸników do obiektów President
    President* presidentsList[MAX_PRESIDENTS];

    int presidentCount = 0;     //liczba wczytanych prezydentów

    // Wywo³ujemy nasz¹ funkcjê, aby wczyta³a dane do tablicy
    loadData("presidents.txt", presidentsList, presidentCount);

    if (presidentCount > 0) {                                   //sprawdzenie czy dzia³a
        printPresidents(presidentsList, presidentCount);
        //zad2
        findShortestTermPresidents(presidentsList,presidentCount);

        //zad3
        string party;
        cout << endl << "Podaj partie z jakiej chcesz znalezc prezydenta:  ";
        getline(cin, party);
        findPresidentsByParty(presidentsList, presidentCount, party);

        //zad4
        int year;
        cout << endl << "Podaj rok w ktorym chcesz sprawdzic kto byl prezydentem:  ";
        cin >> year;
        findPresidentsByYear(presidentsList, presidentCount, year);
    }


    // Zwalnianie pamiêci 
    cout << "\nZwalnianie pamieci..." << endl;
    for (int i = 0; i < presidentCount; i++) {
        delete presidentsList[i]; // Usuwamy obiekt, na który wskazuje wskaŸnik
        presidentsList[i] = nullptr; // Dobra praktyka: ustawiamy wskaŸnik na pusty
    }
    cout << "Pamiec zwolniona." << endl;

    return 0;
}


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <type_traits>
#include <chrono>
#include <algorithm>

using namespace std;
//pliki do ktorych zapisywane byly wyniki
ofstream plik("bubble.txt");
ofstream plik2("insert.txt");
ofstream plik3("merge.txt");

//zadeklarowane tablice w ktorych beda przechowywane aktualnie generowane wartosci
template<typename T>
T* glowna_tablica = nullptr;

template<typename T>
int glowna_tab_rozmiar = 0;

template<typename T>
T* tablica = nullptr;

template<typename T>
int rozmiar_tablicy = 0;

//funkcja generujaca losowe wartosci dla typow int - 1 do 10000, dla typow float 0.0 do 1.0
template<typename T>
T* generujLosoweLiczby(int n) {
    srand(time(NULL));
    T* tablica = new T[n];
    if constexpr (is_same<T, int>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = rand() % 10000 + 1;
        }
    } else if constexpr (is_same<T, float>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
        }
    }
    cout << "Wygenerowano liczby" << endl;
    return tablica;
}

//funkcja generujaca rosnace liczby dla odpowiednich typow danych
template<typename T>
T* generujRosnaceLiczby(int n) {
    T* tablica = new T[n];

    if constexpr (is_same<T, int>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = i + 1;
        }
    } else if constexpr (is_same<T, float>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = static_cast<T>(i + 1) + 0.5;
        }
    }
    cout << "Wygenerowano liczby rosnace" << endl;
    return tablica;
}


//funkcja generujaca malejace liczby dla odpowiednich typow danych
template<typename T>
T* generujMalejaceLiczby(int n) {
    T* tablica = new T[n];
    if constexpr (is_same<T, int>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = n - i;
        }
    } else if constexpr (is_same<T, float>::value) {
        for (int i = 0; i < n; ++i) {
            tablica[i] = static_cast<T>(n - i) - 0.5;
        }
    }
    cout << "Wygenerowano liczby malejace" << endl;
    return tablica;
}

int* generateData(int n) {
    int* data = new int[n];
    // Generowanie pierwszych 60% danych w posortowanej kolejności
    for (int i = 0; i < n * 0.6; ++i) {
        data[i] = i;
    }
    // Generowanie pozostałych 40% danych w losowej kolejności
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(data[static_cast<int>(n * 0.6) - 1] + 1, INT_MAX);
    for (int i = static_cast<int>(n * 0.6); i < n; ++i) {
        data[i] = dis(gen);
    }
    // Wymieszanie tablicy, aby uzyskać losową kolejność dla ostatnich 40% danych
    std::shuffle(data + static_cast<int>(n * 0.6), data + n, gen);
    return data;
}

//funkcja wyswietlajaca wszystkie elementy tablicy, ktora jest podawana jako argument
template<typename T>
void wyswietl(T tablica[], int rozmiar_tablicy) {
    if (tablica == nullptr) {
        cout << "Tablica jest pusta." << endl;
        return;
    }
    for (int i = 0; i < rozmiar_tablicy; ++i) {
        cout << tablica[i] << " ";
    }
    cout << endl;
}

//kolejne algorytny sortowania
template<typename T>
void bubbleSort(T arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

template<typename T>
void insertion(T arr[], int n) {
    for (int i = 1; i < n; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

//funkcja pomocnicza do sortowania przez scalanie
template<typename T>
void merge(T arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    T L[n1], R[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

template<typename T>
void mergeSort(T arr[], int l, int r) {
    if (l >= r)
        return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

//funkcja wczytujaca plik znajdujacy sie w katalogu cmake-build-debug
template<typename T>
void opcja1() {
    cout << "Wybrano wczytanie tablicy z pliku" << endl;
    string nazwa_pliku;
    cout << "Podaj nazwe pliku do odczytu: ";
    cin >> nazwa_pliku;
    ifstream plik(nazwa_pliku);
    if (!plik) {
        cerr << "Nie mozna otworzyć pliku." << endl;
        return;
    }
    delete[] tablica<T>;
    tablica<T> = nullptr;
    rozmiar_tablicy<T> = 0;
    T liczba;
    //dynamiczna alokacja pamieci do pliku
    //wczytywanie konczy sie, gdy wszystkie dane zostana wczytane
    while (plik >> liczba) {
        T* nowa_tablica = new T[rozmiar_tablicy<T> + 1];
        for (int i = 0; i < rozmiar_tablicy<T>; ++i) {
            nowa_tablica[i] = tablica<T>[i];
        }
        nowa_tablica[rozmiar_tablicy<T>] = liczba;
        delete[] tablica<T>;
        tablica<T> = nowa_tablica;
        ++rozmiar_tablicy<T>;
    }
    cout << "Wczytano tablice o nazwie " << nazwa_pliku << endl;
    glowna_tablica<T> = tablica<T>;
    glowna_tab_rozmiar<T> = rozmiar_tablicy<T>;
    plik.close();
}

template<typename T>
void opcja2() {
    cout << "Wybrano wyswietlenie aktualnej tablicy" << endl;
    wyswietl(glowna_tablica<T>, glowna_tab_rozmiar<T>);
}

//opcja3, opcja4, opcja5 to kolejne funkcje sortujace, wyswietlajace czas sortowania, oraz zapisujace ten czas do pliku
//funkcje te dodatkowo wyswietlaja tablice, aktualnie funkcje wyswietlajace tablice sa wykomentowane, ze wzgledu na duzy rozmiar testowanych tablic
template<typename T>
void opcja3() {
    if (tablica<T> == nullptr) {
        cout << "Tablica jest pusta." << endl;
        return;
    }

    T* kopia_tablicy = new T[rozmiar_tablicy<T>];
    for (int i = 0; i < rozmiar_tablicy<T>; ++i) {
        kopia_tablicy[i] = tablica<T>[i];
    }

    //wyswietl(glowna_tablica<T>,glowna_tab_rozmiar<T>);
    auto start = chrono::steady_clock::now();
    bubbleSort(kopia_tablicy, rozmiar_tablicy<T>);
    auto end = chrono::steady_clock::now();
    auto czas_wykonania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas wykonania funkcji: " << czas_wykonania << " ms" << endl;
    if (plik.is_open()){
        plik << czas_wykonania << endl;
    }
    //wyswietl(kopia_tablicy,rozmiar_tablicy<T>);

    delete[] kopia_tablicy;
    cout << "Tablica zostala posortowana." << endl;
}

template<typename T>
void opcja4() {
    if (tablica<T> == nullptr) {
        cout << "Tablica jest pusta." << endl;
        return;
    }

    T* kopia_tablicy = new T[rozmiar_tablicy<T>];
    for (int i = 0; i < rozmiar_tablicy<T>; ++i) {
        kopia_tablicy[i] = tablica<T>[i];
    }

    //wyswietl(glowna_tablica<T>,glowna_tab_rozmiar<T>);
    auto start = chrono::steady_clock::now();
    insertion(kopia_tablicy, rozmiar_tablicy<T>);
    auto end = chrono::steady_clock::now();
    auto czas_wykonania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas wykonania funkcji: " << czas_wykonania << " ms" << endl;
    if (plik2.is_open()){
        plik2 << czas_wykonania << endl;
    }
    //wyswietl(kopia_tablicy,rozmiar_tablicy<T>);

    delete[] kopia_tablicy;
    cout << "Tablica zostala posortowana." << endl;
};

template<typename T>
void opcja5() {
    if (tablica<T> == nullptr) {
        cout << "Tablica jest pusta." << endl;
        return;
    }

    T* kopia_tablicy = new T[rozmiar_tablicy<T>];
    for (int i = 0; i < rozmiar_tablicy<T>; ++i) {
        kopia_tablicy[i] = tablica<T>[i];
    }
    //wyswietl(glowna_tablica<T>,glowna_tab_rozmiar<T>);
    auto start = chrono::steady_clock::now();
    mergeSort(kopia_tablicy, 0, rozmiar_tablicy<T> - 1);
    auto end = chrono::steady_clock::now();
    auto czas_wykonania = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Czas wykonania funkcji: " << czas_wykonania << " ms" << endl;
    if (plik3.is_open()){
        plik3 << czas_wykonania << endl;
    }
    //wyswietl(kopia_tablicy,rozmiar_tablicy<T>);

    delete[] kopia_tablicy;
    cout << "Tablica zostala posortowana." << endl;
};

template<typename T>
void opcja6() {
    int n;
    cout << "Podaj ilosc liczb do wygenerowania: ";
    cin >> n;
    if (n < 2) {
        cout << "Podano zla wartosc";
    } else {
        glowna_tablica<T> = generujLosoweLiczby<T>(n);
        glowna_tab_rozmiar<T> = n;
        tablica<T> = glowna_tablica<T>;
        rozmiar_tablicy<T> = glowna_tab_rozmiar<T>;
    }
};

void wyswietlMenu() {
    cout << "Menu:\n";
    cout << "1. Wczytaj liczby z pliku\n";
    cout << "2. Wyswietl aktualna zapisana tablice\n";
    cout << "3. Sortowanie babelkowe\n";
    cout << "4. Sortowanie przez wstawianie\n";
    cout << "5. Sortowanie przez scalanie\n";
    cout << "6. Wygeneruj losowe n liczb\n";
    cout << "7. Zmiana typu danych\n";
    cout << "8. Symulacja dla losowych danych\n";
    cout << "9. Generowanie rosnacych liczb\n";
    cout << "10. Generowanie malejacych liczb\n";
    cout << "11. Generowanie tablicy z 60% posortowania liczb\n";
    cout << "0. Wyjscie\n";
    cout << "Wybierz opcje: ";
}

int main() {
    // zmienna przechowujaca informacje o wybranym typie danych
    bool czy_float = false;
    int opcja;
    do {
        if (czy_float){
            cout << "Typ Float" << endl;
        } else{
            cout << "Typ Int" << endl;
        }
        wyswietlMenu();
        cin >> opcja;
        switch (opcja) {
            case 1:
                if (czy_float) opcja1<float>();
                else opcja1<int>();
                break;
            case 2:
                if (czy_float) opcja2<float>();
                else opcja2<int>();
                break;
            case 3:
                if (czy_float) opcja3<float>();
                else opcja3<int>();
                break;
            case 4:
                if (czy_float) opcja4<float>();
                else opcja4<int>();
                break;
            case 5:
                if (czy_float) opcja5<float>();
                else opcja5<int>();
                break;
            case 6:
                if (czy_float) opcja6<float>();
                else opcja6<int>();
                break;
            case 0:
                cout << "Wyjscie z programu." << endl;
                break;
            case 7: // opcja zmiany typu danych
                czy_float = !czy_float; // zmiana typu danych
                cout << "Zmieniono typ danych na " << (czy_float ? "float" : "int") << endl;
                break;
            case 8:
                cout << "Symulacja kolejnych sortowan dla aktualnie wybranego typu danych" << endl;
                int dane;
                cout << "Podaj rozmiar danych" << endl;
                cin >> dane;
                int petla;
                cout << "Podaj ilosc iteracji petli" << endl;
                cin >> petla;
                if (czy_float){
                    cout << "Symulacja dla danych typu float" << endl;
                    for (int i = 0; i < petla; i++){
                        cout << "Petla nr " << i+1 << endl;
                        glowna_tablica<float> = generujLosoweLiczby<float>(dane);
                        glowna_tab_rozmiar<float> = dane;
                        tablica<float> = glowna_tablica<float>;
                        rozmiar_tablicy<float> = glowna_tab_rozmiar<float>;
                        opcja3<float>();
                        opcja4<float>();
                        opcja5<float>();
                    }
                }else{
                    cout << "Symulacja dla danych typu int" << endl;
                    for (int i = 0; i < petla; i++){
                        cout << "Petla nr " << i+1 << endl;
                        glowna_tablica<int> = generujLosoweLiczby<int>(dane);
                        glowna_tab_rozmiar<int> = dane;
                        tablica<int> = glowna_tablica<int>;
                        rozmiar_tablicy<int> = glowna_tab_rozmiar<int>;
                        opcja3<int>();
                        opcja4<int>();
                        opcja5<int>();
                    }
                }
            case 9:
                int rozmiar;
                cout << "Podaj rozmiar danych: ";
                cin >> rozmiar;
                if (czy_float){
                    glowna_tablica<float> = generujRosnaceLiczby<float>(rozmiar);
                    glowna_tab_rozmiar<float> = rozmiar;
                    tablica<float> = glowna_tablica<float>;
                    rozmiar_tablicy<float> = glowna_tab_rozmiar<float>;
                }else{
                    glowna_tablica<int> = generujRosnaceLiczby<int>(rozmiar);
                    glowna_tab_rozmiar<int> = rozmiar;
                    tablica<int> = glowna_tablica<int>;
                    rozmiar_tablicy<int> = glowna_tab_rozmiar<int>;
                }
                cout << "Wygenerowano rosnace liczby." << endl;
                break;
            case 10:
                int size;
                cout << "Podaj rozmiar danych: ";
                cin >> size;
                if (czy_float){
                    glowna_tablica<float> = generujMalejaceLiczby<float>(size);
                    glowna_tab_rozmiar<float> = size;
                    tablica<float> = glowna_tablica<float>;
                    rozmiar_tablicy<float> = glowna_tab_rozmiar<float>;
                }else{
                    glowna_tablica<int> = generujMalejaceLiczby<int>(size);
                    glowna_tab_rozmiar<int> = size;
                    tablica<int> = glowna_tablica<int>;
                    rozmiar_tablicy<int> = glowna_tab_rozmiar<int>;
                }
                cout << "Wygenerowano malejace liczby." << endl;
                break;
            case 11:
                int el;
                cout << "Podaj liczbe elementow " << endl;
                cin >> el;
                if (!czy_float){
                    glowna_tablica<int> = generateData(el);
                    glowna_tab_rozmiar<int> = el;
                    tablica<int> = glowna_tablica<int>;
                    rozmiar_tablicy<int> = glowna_tab_rozmiar<int>;
                    cout << "Wygenerowano czesciowa tablice" << endl;
                } else{
                    cout << "Zmien typ danych na int, aby wygenerowac czesciowa tablice" << endl;
                }
            default:
                cout << "Niepoprawna opcja. Wybierz ponownie." << endl;
                break;
        }
    } while (opcja != 0);
    // Zwolnienie pamięci po zakończeniu programu
    if (czy_float) {
        delete[] tablica<float>;
        delete[] glowna_tablica<float>;
    } else {
        delete[] tablica<int>;
        delete[] glowna_tablica<int>;
    }

    return 0;
}

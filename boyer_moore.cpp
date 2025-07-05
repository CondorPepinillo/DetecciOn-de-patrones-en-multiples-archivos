//Implementacion de Boyer-Moore String Matching Algorithm de cortesia de GeeksforGeeks,
//la cual implementaremos junto a nuestro main para que leea un archivo de texto (de nuestros datasets por ejemplo) y busque patrones en el mismo.
//se alterara el codigo para en lugar de retornar los lugares en que encuentra el patron en su lugar imprima cuantas veces se encuentra el patron en el texto.

/* C++ Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <map>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"
class BoyerMoore
{
public:
    // The preprocessing function for Boyer Moore's
    // bad character heuristic
    #define NO_OF_CHARS 256

void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (int i = 0; i < size; i++)
        badchar[(int) str[i]] = i;
}

void search(string txt, string pat, map<int, int>& section_counts) {
    int m = pat.size();
    int n = txt.size();

    int badchar[NO_OF_CHARS];
    badCharHeuristic(pat, m, badchar);

    int s = 0;
    int current_section = 1;
    vector<int> position_to_section(n);

    // Precompute section mapping
    for (int i = 0; i < n-1; i++) {
        if (txt[i] == '\x7F') {
            current_section++;
        }
        position_to_section[i] = current_section;
    }

    // Initialize section_counts with 0
    for (int sec = 1; sec <= current_section; sec++) {
        section_counts[sec] = 0;
    }

    while (s <= (n - m)) {
        int j = m - 1;

        while (j >= 0 && pat[j] == txt[s + j])
            j--;

        if (j < 0) {
            int section = position_to_section[s];
            section_counts[section]++; // Increment count for this section

            s += (s + m < n) ? m - badchar[txt[s + m]] : 1;
        }
        else {
            s += max(1, j - badchar[txt[s + j]]);
        }
    }
}
};
/* Driver code */
int main(int argc, char* argv[])
{
    
    if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
    return 0;
    }

    string separador ="\x7F";
    string textoDondeBuscar = toString(argc - 1, &argv[1], separador);
    cout << "Texto concatenado tiene " << textoDondeBuscar.size() << " caracteres." << endl;

    int count = 0;

    // Read each line of the file, store
    // it in string s and print it to the
    // standard output stream 

    //Almacenar patrones a buscar en un vector
    string archivoPatrones = argv[argc - 1];
    vector<string> patrones;
    ifstream filePatrones(archivoPatrones);
    if (!filePatrones.is_open()) {
        cerr << "No se pudo abrir el archivo" << archivoPatrones << endl;
        return 1;
    }
    string linea;
    while (getline(filePatrones, linea)) {
        if (!linea.empty())
            patrones.push_back(linea);
    }
    filePatrones.close();


    BoyerMoore bm;

    cout << "Buscando patrones..." << endl;
    for (const auto& patron : patrones) {
    
        map<int, int> section_counts;
        
        auto start = chrono::high_resolution_clock::now();
        bm.search(textoDondeBuscar, patron, section_counts);
        auto end = chrono::high_resolution_clock::now();

        
        double running_time = chrono::duration<double>(end - start).count();
        for (const auto& pair : section_counts) {
        cout << "Texto " << pair.first << ". Patron: "<<patron<< ": " << pair.second << " occurrence(s)" << "en" << running_time << "segundos."<< endl;
    }

    }

    return 0;
}

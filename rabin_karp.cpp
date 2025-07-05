// Rabin-Karp Algorithm for Pattern Searching in C++
// Reference: Introduction to Algorithms (CLRS)
//Implementacion de Rabin-Karp de cortesia de GeeksforGeeks,
//la cual implementaremos junto a nuestro main para que leea un archivo de texto (de nuestros datasets por ejemplo) y busque patrones en el mismo.
//se alterara el codigo para en lugar de retornar los lugares en que encuentra el patron en su lugar imprima cuantas veces se encuentra el patron en el texto.
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "toString.cpp"
#include <map>
using namespace std;

class RabinKarp {
public:
    map<int, int> search(const string &txt, const string &pat,map<int, int>& section_counts, char separator = '\x7F') {
        int d = 256;  // Number of characters in input alphabet
        int q = 101;  // A prime number for hashing
        int M = pat.length();
        int N = txt.length();
        int p = 0;    // Hash value for pattern
        int t = 0;    // Hash value for text window
        int h = 1;    // High-order digit multiplier

        // Map from position to section
        int current_section = 1;
        vector<int> position_to_section(N);
        for (int i = 0; i < N-1; i++) {
            if (txt[i] == separator) {
                current_section++;
            }
            position_to_section[i] = current_section;
        }

        
        for (int sec = 1; sec <= current_section; sec++) {
            section_counts[sec] = 0;
        }

        // Precompute h = pow(d, M-1) % q
        for (int i = 0; i < M - 1; i++) {
            h = (h * d) % q;
        }

        // Initial hash values for pattern and first window
        for (int i = 0; i < M; i++) {
            p = (d * p + pat[i]) % q;
            t = (d * t + txt[i]) % q;
        }

        // Slide the pattern over text
        for (int i = 0; i <= N - M; i++) {
            if (p == t) {
                bool match = true;
                for (int j = 0; j < M; j++) {
                    if (txt[i + j] != pat[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    int section = position_to_section[i];
                    section_counts[section]++; // Increment count for this section
                }
            }

            // Compute hash for next window
            if (i < N - M) {
                t = (d * (t - txt[i] * h) + txt[i + M]) % q;
                if (t < 0)
                    t += q; // Ensure non-negative
            }
        }

        return section_counts;
    }
};

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

    RabinKarp rp;

    cout << "Buscando patrones usando Rabin-Karp..." << endl;
    for (const auto& patron :patrones){
        map<int, int> section_counts;

        auto start = chrono::high_resolution_clock::now();
        rp.search(textoDondeBuscar, patron, section_counts);
        auto end = chrono::high_resolution_clock::now();

        double running_time = chrono::duration<double>(end - start).count();
        for (const auto& pair : section_counts) {
            cout << "Texto " << pair.first << ". Patron: " << patron << ": " << pair.second << " occurrence(s) en " << running_time << " segundos." << endl;
        }
    }

    return 0;
}
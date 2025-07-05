// C++ program to search the pattern in given text using
// KMP Algorithm

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <map>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"

class KMP{
    public:
        void constructLps(string &pat, vector<int> &lps) {
            // len stores the length of longest prefix which
            // is also a suffix for the previous index
            int len = 0;
            // lps[0] is always 0
            lps[0] = 0;
            size_t i = 1;
            while (i < pat.length()){
                // If characters match, increment the size of lps
                if (pat[i] == pat[len]) {
                    len++;
                    lps[i] = len;
                    i++;
                }
                // If there is a mismatch
                else {
                    if (len != 0) {
                        // Update len to the previous lps value
                        // to avoid reduntant comparisons
                        len = lps[len - 1];
                    }
                    else {
                        // If no matching prefix found, set lps[i] to 0
                        lps[i] = 0;
                        i++;
                    }
                }
            }
    }

        map<int, int> search(const string &pat, const string &txt, char separator = '\x7F') {
            int n = txt.length();
            int m = pat.length();

            vector<int> lps(m);
            constructLps(const_cast<string&>(pat), lps);

            map<int, int> section_counts;
            int current_section = 1;
            vector<int> position_to_section(n);

            // Precompute mapping de posiciones a secciones
            // Hasta n-1 porque el separador del final crearia una sección vacía
            for (int i = 0; i < n-1; i++) {
                if (txt[i] == separator) {
                    current_section++;
                }
                position_to_section[i] = current_section;
            }

            // Inicializa todos los conteos a 0
            for (int sec = 1; sec <= current_section; sec++) {
                section_counts[sec] = 0;
            }

            int i = 0, j = 0;

            while (i < n) {
                if (txt[i] == pat[j]) {
                    i++;
                    j++;

                    if (j == m) {
                        // Ocurre una coincidencia completa
                        int start_pos = i - j;
                        int section = position_to_section[start_pos];
                        section_counts[section]++; // Incrementa la cuenta en esa sección

                        j = lps[j - 1]; // Sigue buscando
                    }
                } else {
                    if (j != 0) {
                        j = lps[j - 1];
                    } else {
                        i++;
                    }
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
        cerr << "No se pudo abrir el archivo de patrones: " << archivoPatrones << endl;
        return 1;
    }
    string linea;
    while (getline(filePatrones, linea)) {
        if(!linea.empty()) {
            patrones.push_back(linea);
        }
    }

    filePatrones.close();
    
    KMP kmp;

    cout << "Buscando patrones..." << endl;

    for(const auto& patron : patrones){
        map<int, int> section_counts;

        auto start = chrono::high_resolution_clock::now();
        section_counts = kmp.search(patron, textoDondeBuscar);
        auto end = chrono::high_resolution_clock::now();

        double running_time = chrono::duration<double>(end - start).count();
        for (const auto& pair : section_counts) {
            cout << "Texto " << pair.first << ". Patron: "<<patron<< ": " << pair.second << " occurrence(s)" << "en" << running_time << "segundos."<< endl;
        }
    }

    
    return 0;

}
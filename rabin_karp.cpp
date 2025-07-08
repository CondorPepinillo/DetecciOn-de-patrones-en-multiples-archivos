// Rabin-Karp Algorithm for Pattern Searching in C++
// Implementación adaptada con soporte multiarchivo y conteo por secciones
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <map>
#include <dirent.h>
using namespace std;
#include "toString.cpp"
#include "getName.cpp"

class RabinKarp {
public:
    void search(const string &txt, const string &pat, map<int, int>& section_counts, char separator = '\x7F') {
        int d = 256;  // Number of characters in input alphabet
        int q = 101;  // A prime number for hashing
        int M = pat.length();
        int N = txt.length();
        int p = 0;    // Hash value for pattern
        int t = 0;    // Hash value for text window
        int h = 1;    // High-order digit multiplier

        // Map position to section
        int current_section = 1;
        vector<int> position_to_section(N);
        for (int i = 0; i < N; i++) {
            if (txt[i] == separator) {
                current_section++;
            }
            position_to_section[i] = current_section;
        }

        // Inicializa conteo de todas las secciones
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
                    if (i >= 0 && i < position_to_section.size()) {
                        int section = position_to_section[i];
                        if (section >= 1 && section <= current_section) {
                            section_counts[section]++;
                        }
                    }
                }
            }

            // Compute hash for next window
            if (i < N - M) {
                t = (d * (t - txt[i] * h) + txt[i + M]) % q;
                if (t < 0)
                    t += q; // Ensure non-negative
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <carpeta> <n> <archivo_patrones>\n";
        return 0;
    }

    string carpeta = argv[1];
    int n = stoi(argv[2]);
    string archivoPatrones = argv[3];
    string separador = "\x7F";
    vector<string> archivos;

    // Leer solo los primeros n archivos de la carpeta
    DIR* dir;
    struct dirent* ent;
    int count = 0;
    if ((dir = opendir(carpeta.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string nombre = ent->d_name;
            if (nombre != "." && nombre != "..") {
                string ruta = carpeta + "/" + nombre;
                ifstream file(ruta);
                if (file.is_open()) {
                    archivos.push_back(ruta);
                    count++;
                    if (count >= n) break;
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "No se pudo abrir la carpeta: " << carpeta << endl;
        return 1;
    }

    // Concatenar archivos
    vector<const char*> archivos_cstr;
    for (const auto& archivo : archivos) {
        archivos_cstr.push_back(archivo.c_str());
    }
    string textoDondeBuscar = toString(archivos_cstr.size(), archivos_cstr.data(), separador);
    cout << "Texto concatenado tiene " << textoDondeBuscar.size() << " caracteres." << endl;

    // Leer patrones
    vector<string> patrones;
    ifstream filePatrones(archivoPatrones);
    if (!filePatrones.is_open()) {
        cerr << "No se pudo abrir el archivo de patrones: " << archivoPatrones << endl;
        return 1;
    }
    string linea;
    while (getline(filePatrones, linea)) {
        if (!linea.empty())
            patrones.push_back(linea);
    }
    filePatrones.close();

    RabinKarp rk;
    cout << "Buscando patrones usando Rabin-Karp..." << endl;

    for (const auto& patron : patrones) {
        map<int, int> section_counts;
        auto start = chrono::high_resolution_clock::now();
        rk.search(textoDondeBuscar, patron, section_counts);
        auto end = chrono::high_resolution_clock::now();

        double running_time = chrono::duration<double>(end - start).count();
        for (const auto& pair : section_counts) {
            int idx = pair.first - 1;

            if (idx >= 0 && idx < archivos.size()) {
                vector<string> parts = split(archivos[idx], '\\'); // o "\\" en Windows
                cout << "Texto " << parts.back()
                     << ", Patron: \"" << patron
                     << " : " << pair.second
                     << " occurrence(s) en " << running_time << " segundos." << endl;
            }
        }
    }

    return 0;
}
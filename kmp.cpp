// Implementación de KMP String Matching Algorithm
// Adaptado para leer archivos desde carpeta y contar ocurrencias por archivo
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <map>
#include <dirent.h>
using namespace std;
#include "toString.cpp"
#include "getName.cpp"

class KMP {
public:
    void constructLps(string &pat, vector<int> &lps) {
        int len = 0;
        lps[0] = 0;
        size_t i = 1;
        while (i < pat.length()) {
            if (pat[i] == pat[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) {
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }

    void search(const string &pat, const string &txt, map<int, int> &section_counts, char separator = '\x7F') {
        int n = txt.length();
        int m = pat.length();

        vector<int> lps(m);
        constructLps(const_cast<string &>(pat), lps);

        int current_section = 1;
        vector<int> position_to_section(n);

        // Precompute mapping de posiciones a secciones
        for (int i = 0; i < n; i++) {
            if (txt[i] == separator) {
                current_section++;
            }
            position_to_section[i] = current_section;
        }

        // Inicializar conteos
        for (int sec = 1; sec <= current_section; sec++) {
            section_counts[sec] = 0;
        }

        int i = 0, j = 0;

        while (i < n) {
            if (txt[i] == pat[j]) {
                i++;
                j++;
                if (j == m) {
                    int start_pos = i - j;
                    if (start_pos >= 0 && start_pos < position_to_section.size()) {
                        int section = position_to_section[start_pos];
                        if (section >= 1 && section <= current_section) {
                            section_counts[section]++;
                        } else {
                            cerr << "[WARN] Sección inválida: " << section << " en posición=" << start_pos << endl;
                        }
                    } else {
                        cerr << "[WARN] Posición fuera de rango: start_pos=" << start_pos << endl;
                    }
                    j = lps[j - 1];
                }
            } else {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <carpeta> <n> <archivo_patrones>\n";
        return 1;
    }

    string carpeta = argv[1];
    int n = stoi(argv[2]);
    string archivoPatrones = argv[3];
    string separador = "\x7F";
    vector<string> archivos;

    // Leer primeros n archivos de la carpeta usando dirent
    DIR *dir;
    struct dirent *ent;
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
    vector<const char *> archivos_cstr;
    for (const auto &archivo : archivos) {
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

    KMP kmp;
    cout << "Buscando patrones..." << endl;

    for (const auto &patron : patrones) {
        map<int, int> section_counts;
        auto start = chrono::high_resolution_clock::now();
        kmp.search(patron, textoDondeBuscar, section_counts);
        auto end = chrono::high_resolution_clock::now();

        double running_time = chrono::duration<double>(end - start).count();
        for (const auto &pair : section_counts) {
            int idx = pair.first - 1;

            // Validar índice antes de usarlo
            if (idx >= 0 && idx < archivos.size()) {
                vector<string> parts = split(archivos[idx], '\\'); // compatible UNIX
                cout << parts.back()<<";" << patron<<";" << pair.second<<";" << running_time <<";"<< endl;
            } else {
                cerr << "[WARN] Índice fuera de rango en archivos: " << pair.first << " (idx=" << idx << ")" << endl;
            }
        }
    }

    return 0;
}

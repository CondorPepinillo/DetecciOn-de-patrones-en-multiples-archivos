#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <dirent.h>
#include "toString.cpp"
#include "getName.cpp"
using namespace std;

#ifndef FMINDEX_HPP
#define FMINDEX_HPP

using namespace std;

class FMIndex {
private:
    string bwt;
    map<char, int> C;
    map<char, vector<int>> Occ;
    vector<int> suffixArray;

    string buildBWT(const string &text) {
        int n = text.size();
        suffixArray.resize(n);

        for (int i = 0; i < n; i++)
            suffixArray[i] = i;

        sort(suffixArray.begin(), suffixArray.end(), [&](int a, int b) {
            return text.substr(a) < text.substr(b);
        });

        string result;
        for (int i = 0; i < n; i++) {
            int idx = suffixArray[i];
            result += (idx == 0) ? text[n - 1] : text[idx - 1];
        }
        return result;
    }

    map<char, int> buildC(const string &bwt) {
        map<char, int> freq, C;
        for (char c : bwt)
            freq[c]++;

        int total = 0;
        for (auto it = freq.begin(); it != freq.end(); ++it) {
            char c = it->first;
            int f = it->second;
            C[c] = total;
            total += f;
        }
        return C;
    }

    map<char, vector<int>> buildOcc(const string &bwt) {
        map<char, vector<int>> occ;
        map<char, int> counter;

        for (char c : bwt) {
            if (occ.find(c) == occ.end()) {
                occ[c].push_back(0);
                counter[c] = 0;
            }
        }

        for (char c : bwt) {
            counter[c]++;
            for (auto it = occ.begin(); it != occ.end(); ++it) {
                char ch = it->first;
                vector<int> &v = it->second;
                v.push_back(counter[ch]);
            }
        }

        for (auto it = occ.begin(); it != occ.end(); ++it) {
            vector<int> &v = it->second;
            if (v.size() < bwt.size() + 1) {
                int last = v.empty() ? 0 : v.back();
                v.resize(bwt.size() + 1, last);
            }
        }

        return occ;
    }

public:
    FMIndex(const string &text) {
        string textWithEOF = text + "$";
        bwt = buildBWT(textWithEOF);
        C = buildC(bwt);
        Occ = buildOcc(bwt);
    }

    // Cuenta cuántas veces aparece el patrón
    int count(const string &pattern) const {
        int l = 0;
        int r = bwt.size();

        for (int i = pattern.size() - 1; i >= 0; i--) {
            char c = pattern[i];
            if (C.find(c) == C.end()) return 0;

            l = C.at(c) + Occ.at(c)[l];
            r = C.at(c) + Occ.at(c)[r];
            if (l >= r) return 0;
        }

        return r - l;
    }

    vector<int> findPositions(const string &pattern) const {
        int l = 0;
        int r = bwt.size();

        for (int i = pattern.size() - 1; i >= 0; i--) {
            char c = pattern[i];
            if (C.find(c) == C.end()) return {};
            l = C.at(c) + Occ.at(c)[l];
            r = C.at(c) + Occ.at(c)[r];
            if (l >= r) return {};
        }

        vector<int> posiciones;
        for (int i = l; i < r; i++) {
            posiciones.push_back(suffixArray[i]);
        }
        return posiciones;
    }
};

#endif

// ...existing includes and FMIndex class...

// ...includes y clase FMIndex como ya tienes...

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

    // Leer solo los primeros n archivos de la carpeta usando dirent
    DIR* dir;
    struct dirent* ent;
    int fileCount = 0;
    if ((dir = opendir(carpeta.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string nombre = ent->d_name;
            if (nombre != "." && nombre != "..") {
                string ruta = carpeta + "/" + nombre;
                ifstream file(ruta);
                if (file.is_open()) {
                    archivos.push_back(ruta);
                    fileCount++;
                    if (fileCount >= n) break;
                }
            }
        }
        closedir(dir);
    } else {
        cerr << "No se pudo abrir la carpeta: " << carpeta << endl;
        return 1;
    }

    // Concatenar archivos usando toString
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
        cerr << "No se pudo abrir el archivo " << archivoPatrones << endl;
        return 1;
    }
    string linea;
    while (getline(filePatrones, linea)) {
        if (!linea.empty())
            patrones.push_back(linea);
    }
    filePatrones.close();

    // Construir FMIndex
    FMIndex fm(textoDondeBuscar);

    // Mapeo de posiciones a archivos usando el separador
    vector<int> pos_to_archivo(textoDondeBuscar.size());
    int archivo_actual = 0;
    for (size_t i = 0; i < textoDondeBuscar.size(); ++i) {
        if (textoDondeBuscar[i] == '\x7F') archivo_actual++;
        pos_to_archivo[i] = archivo_actual;
    }

    cout << "Buscando patrones..." << endl;

for (const auto& patron : patrones) {
    map<int, int> section_counts;
    auto start = chrono::high_resolution_clock::now();
    vector<int> posiciones = fm.findPositions(patron);
    auto end = chrono::high_resolution_clock::now();

    for (int pos : posiciones) {
        int section = count(textoDondeBuscar.begin(), textoDondeBuscar.begin() + pos, '\x7F');
        if (section < 0 || section >= archivos.size()) {
            cerr << "[DEBUG] Posición fuera de rango: pos=" << pos << ", section=" << section << ", archivos.size()=" << archivos.size() << endl;
            continue;
        }
        section_counts[section]++;
    }

    double running_time = chrono::duration<double>(end - start).count();

    for (int idx = 0; idx < archivos.size(); ++idx) {
        int ocurrencias = section_counts.count(idx) ? section_counts[idx] : 0;
        vector<string> parts = split(archivos[idx], '\\');
        cout << parts.back()<<";" << patron<<";" << pair.second<<";" << running_time <<";"<< endl;
}
}
    return 0;
}
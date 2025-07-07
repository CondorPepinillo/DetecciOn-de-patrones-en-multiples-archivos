#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <dirent.h>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"
#include "getName.cpp"

vector<int> buildSufArr(const string &s) {
    int n = s.length();
    vector<int> sufArr(n);

    for (int i = 0; i < n; i++) {
        sufArr[i] = i;
    }

    sort(sufArr.begin(), sufArr.end(), [&](int a, int b) {
        return s.substr(a) < s.substr(b);
    });
    return sufArr;
}

int findLowerBound(const string &text, const vector<int> &sufArr, const string &pattern) {
    int low = 0, high = sufArr.size();
    while (low < high) {
        int mid = (low + high) / 2;
        string suffix = text.substr(sufArr[mid]);
        if (suffix.compare(0, pattern.size(), pattern) < 0) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int findUpperBound(const string &text, const vector<int> &sufArr, const string &pattern) {
    int low = 0, high = sufArr.size();
    while (low < high) {
        int mid = (low + high) / 2;
        string suffix = text.substr(sufArr[mid]);
        if (suffix.compare(0, pattern.size(), pattern) <= 0) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

int countPatternOccurrences(const string &text, const vector<int> &sufArr, const string &pattern) {
    int lower = findLowerBound(text, sufArr, pattern);
    int upper = findUpperBound(text, sufArr, pattern);
    return upper - lower;
}

/*vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}*/

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

    // Concatenar archivos usando toString
    vector<const char*> archivos_cstr;
    for (const auto& archivo : archivos) {
        archivos_cstr.push_back(archivo.c_str());
    }
    string textoDondeBuscar = toString(archivos_cstr.size(), archivos_cstr.data(), separador);
    cout << "Texto concatenado tiene " << textoDondeBuscar.size() << " caracteres." << endl;

    // Construir el Suffix Array
    vector<int> sufArr = buildSufArr(textoDondeBuscar);

    // Almacenar patrones a buscar en un vector
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

    cout << "Buscando patrones..." << endl;

    // Precomputar mapeo de posición a sección
    vector<int> pos_to_section(textoDondeBuscar.size(), 1);
    int current_section = 1;
    for (size_t i = 0; i < textoDondeBuscar.size(); i++) {
        if (textoDondeBuscar[i] == separador[0]) {
            current_section++;
        }
        pos_to_section[i] = current_section;
    }

    for (const auto& patron : patrones) {
        auto start = chrono::high_resolution_clock::now();
        
        // Buscar todas las ocurrencias del patrón
        int lower = findLowerBound(textoDondeBuscar, sufArr, patron);
        int upper = findUpperBound(textoDondeBuscar, sufArr, patron);
        int total_occurrences = upper - lower;

        // Contar ocurrencias por sección
        map<int, int> section_counts;
        for (int i = lower; i < upper; i++) {
            int pos = sufArr[i];
            int section = pos_to_section[pos];
            section_counts[section]++;
        }

        auto end = chrono::high_resolution_clock::now();
        double running_time = chrono::duration<double>(end - start).count();

        // Mostrar resultados por archivo
        for (const auto& pair : section_counts) {
            int idx = pair.first - 1;
            if (idx >= 0 && idx < archivos.size()) {
                vector<string> parts = split(archivos[idx], '\\');
                cout << "Texto " << parts.back() << ", Patron: " << patron 
                     << ": " << pair.second << " occurrence(s) en " 
                     << running_time << " segundos." << endl;
            } else {
                cerr << "[WARN] Índice fuera de rango en archivos: " 
                     << pair.first << " (idx=" << idx << ")" << endl;
            }
        }
    }
    return 0;
}
//Implementacion de Boyer-Moore String Matching Algorithm de cortesia de GeeksforGeeks,
//la cual implementaremos junto a nuestro main para que leea un archivo de texto (de nuestros datasets por ejemplo) y busque patrones en el mismo.
//se alterara el codigo para en lugar de retornar los lugares en que encuentra el patron en su lugar imprima cuantas veces se encuentra el patron en el texto.

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <map>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"
#include "getName.cpp"
#include <dirent.h>

class BoyerMoore
{
public:
    // The preprocessing function for Boyer Moore's
    // bad character heuristic

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

        while (j >= 0 && (s + j) < n && pat[j] == txt[s + j])
            j--;

        if (j < 0) {
            if (s >= 0 && s < position_to_section.size()) {
                int section = position_to_section[s];
                if (section >= 1 && section <= current_section) {
                        section_counts[section]++;
                }
                else{
                        cerr << "[WARN] Sección inválida: " << section
                            << " en posición s=" << s << endl;
                    }
            } else 
            {
                cerr << "[WARN] Posición s fuera de rango: s=" << s << endl;
            }

            if ((s + m) < n) {
                s += max(1, m - badchar[txt[s + m]]);
            } 
            else {
                s++;
            }
        }
        else {
            s += max(1, j - badchar[txt[s + j]]);
        }
    }
}
};

int main(int argc, char* argv[])
{
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

    //Almacenar patrones a buscar en un vector
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

    BoyerMoore bm;
    cout << "Buscando patrones..." << endl;

    for (const auto& patron : patrones) {
        map<int, int> section_counts;
        auto start = chrono::high_resolution_clock::now();
        bm.search(textoDondeBuscar, patron, section_counts);
        auto end = chrono::high_resolution_clock::now();

        double running_time = chrono::duration<double>(end - start).count();
        for (const auto& pair : section_counts) {
        int idx = pair.first - 1;

        // Validar índice antes de usarlo
        if (idx >= 0 && idx < archivos.size()) {
            //podria ser necesario cambiar el separador dependiendo de la plataforma
            //en windows es '\\' y en linux es '/'
            vector<string> parts = split(archivos[idx], '\\');
            cout << "Texto " << parts.back()<< ", Patron: " << patron<< ": " << pair.second<< " occurrence(s)"<< " en " << running_time << " segundos." << endl;
        } 
        else {
            cerr << "[WARN] Índice fuera de rango en archivos: "<< pair.first << " (idx=" << idx << ")" << endl;
        }
}


        
}
    return 0;
}
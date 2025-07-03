#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

// Construcción de la Burrows-Wheeler Transform
string buildBWT(const string &text, vector<int> &suffixArray) {
    int n = text.size();
    suffixArray.resize(n);

    // Crear suffix array con índices
    for (int i = 0; i < n; i++)
        suffixArray[i] = i;

    // Ordenar por sufijos
    sort(suffixArray.begin(), suffixArray.end(), [&](int a, int b) {
        return text.substr(a) < text.substr(b);
    });

    // Construir BWT
    string bwt;
    for (int i = 0; i < n; i++) {
        int idx = suffixArray[i];
        bwt += (idx == 0) ? text[n - 1] : text[idx - 1];
    }

    return bwt;
}

// Construir tabla C: cuántos caracteres son menores que c
map<char, int> buildC(const string &bwt) {
    map<char, int> freq, C;
    for (int i = 0; i < (int)bwt.size(); i++)
        freq[bwt[i]]++;

    int total = 0;
    for (map<char, int>::iterator it = freq.begin(); it != freq.end(); ++it) {
        C[it->first] = total;
        total += it->second;
    }
    return C;
}

// Construir tabla Occ: frecuencia acumulada de cada carácter
map<char, vector<int> > buildOcc(const string &bwt) {
    map<char, vector<int> > occ;
    map<char, int> counter;

    // Inicializar los vectores con un 0 al principio
    for (int i = 0; i < (int)bwt.size(); i++) {
        char c = bwt[i];
        if (occ.find(c) == occ.end()) {
            vector<int> v;
            v.push_back(0);
            occ[c] = v;
            counter[c] = 0;
        }
    }

    for (int i = 0; i < (int)bwt.size(); ++i) {
        char c = bwt[i];
        counter[c]++;
        for (map<char, vector<int> >::iterator it = occ.begin(); it != occ.end(); ++it) {
            char ch = it->first;
            vector<int> &v = it->second;
            v.push_back(counter[ch]);
        }
    }

    // Asegurar que todos los vectores tengan tamaño bwt.size() + 1
    for (map<char, vector<int> >::iterator it = occ.begin(); it != occ.end(); ++it) {
        vector<int> &v = it->second;
        if (v.size() < bwt.size() + 1) {
            int last = v.empty() ? 0 : v.back();
            v.resize(bwt.size() + 1, last);
        }
    }

    return occ;
}

// Backward search para contar ocurrencias
int countOccurrences(const string &pattern, const string &bwt,
                     const map<char, int> &C,
                     const map<char, vector<int> > &Occ) {
    int l = 0;
    int r = (int)bwt.size();

    for (int i = (int)pattern.size() - 1; i >= 0; i--) {
        char c = pattern[i];
        if (C.find(c) == C.end()) return 0;

        l = C.at(c) + Occ.at(c)[l];
        r = C.at(c) + Occ.at(c)[r];
        if (l >= r) return 0;
    }

    return r - l;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
        return 0;
    }

    // Concatenar el contenido de todos los archivos
    stringstream buffer;
    for (int i = 1; i < argc; ++i) {
        ifstream file(argv[i]);
        if (!file.is_open()) {
            cerr << "No se pudo abrir el archivo: " << argv[i] << endl;
            return 1;
        }
        buffer << file.rdbuf();
        file.close();
    }
    string s = buffer.str();

    string pat = "This";

    auto start = chrono::high_resolution_clock::now();

    // Construir BWT y tablas auxiliares
    vector<int> suffixArray;
    string bwt = buildBWT(s, suffixArray);
    map<char, int> C = buildC(bwt);
    map<char, vector<int>> Occ = buildOcc(bwt);

    // Buscar el patrón usando FM-Index
    int count = countOccurrences(pat, bwt, C, Occ);

    auto end = chrono::high_resolution_clock::now();
    double running_time = chrono::duration<double>(end - start).count();

    cout << "El patron \"" << pat << "\" aparece " << count << " veces en el texto, en: " << running_time << " segundos." << endl;

    return 0;
}
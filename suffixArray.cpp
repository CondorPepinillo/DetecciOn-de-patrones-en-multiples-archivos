// C++ Program to illustrate how to create the
// suffix array
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;
#include "toString.cpp"

vector<int> buildSufArr(string &s) {
    int n = s.length();
    vector<int> sufArr(n);

    // Generating all the suffixes
    for (int i = 0; i < n; i++){
        sufArr[i] = i;
    }

    // Sort all suffixes alphabetically
    sort(sufArr.begin(), sufArr.end(), [&](int a, int b) {
        return s.substr(a) < s.substr(b); // aún se puede optimizar más
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

// Búsqueda binaria para encontrar el primer sufijo > patrón
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

// Cuenta cuántas veces aparece el patrón en el texto
int countPatternOccurrences(const string &text, const vector<int> &sufArr, const string &pattern) {
    int lower = findLowerBound(text, sufArr, pattern);
    int upper = findUpperBound(text, sufArr, pattern);
    return upper - lower;
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
    vector<int> sufArr = buildSufArr(s);

    string pat = "This";

    auto start = chrono::high_resolution_clock::now();
    int count = countPatternOccurrences(s, sufArr, pat);
    auto end = chrono::high_resolution_clock::now();

    double running_time = chrono::duration<double>(end - start).count();

    cout << "El patron \"" << pat << "\" aparece " << count << " veces en el texto, en: " << running_time << " segundos." << endl;

    return 0;
}
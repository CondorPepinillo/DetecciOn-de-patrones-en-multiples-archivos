// C++ Program to illustrate how to create the
// suffix array
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

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

int main() {
    ifstream file("datasets/English/english_00"); // Cambia esto por el archivo que desees
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf(); // Lee todo el contenido del archivo
    string s = buffer.str();
    vector<int> sufArr = buildSufArr(s);

    string pat = "This";


    int count = countPatternOccurrences(s, sufArr, pat);
    cout << "El patron \"" << pat << "\" aparece " << count << " veces en el texto." << endl;

    return 0;
}
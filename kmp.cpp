// C++ program to search the pattern in given text using
// KMP Algorithm

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"

void constructLps(string &pat, vector<int> &lps) {

    // len stores the length of longest prefix which
    // is also a suffix for the previous index
    int len = 0;

    // lps[0] is always 0
    lps[0] = 0;

    size_t i = 1;
    while (i < pat.length()) {

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

vector<int> search(const string &pat, string &txt) {
    int n = txt.length();
    int m = pat.length();

    vector<int> lps(m);
    vector<int> res;

    constructLps(const_cast<string&>(pat), lps);

    // Pointers i and j, for traversing
    // the text and pattern
    int i = 0;
    int j = 0;

    while (i < n) {

        // If characters match, move both pointers forward
        if (txt[i] == pat[j]) {
            i++;
            j++;

            // If the entire pattern is matched
            // store the start index in result
            if (j == m) {
                res.push_back(i - j);

                // Use LPS of previous index to
                // skip unnecessary comparisons
                j = lps[j - 1];
            }
        }

        // If there is a mismatch
        else {

            // Use lps value of previous index
            // to avoid redundant comparisons
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    return res;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
    return 0;
    }

    string separador ="$";
    string textoDondeBuscar = toString(argc - 1, &argv[1], separador);
    string patron = "This";

    int count = 0;

    auto start = chrono::high_resolution_clock::now();
    vector<int> positions = search(patron, textoDondeBuscar);
    count += positions.size();  // suma cantidad de apariciones
    auto end = chrono::high_resolution_clock::now();
    double running_time = chrono::duration<double>(end - start).count();

    cout << "El patrón '" << patron << "' se encontró " << count << " veces en el archivo, en: "<<running_time<<"segundos." << endl;

    return 0;

}
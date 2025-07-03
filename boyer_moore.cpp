//Implementacion de Boyer-Moore String Matching Algorithm de cortesia de GeeksforGeeks,
//la cual implementaremos junto a nuestro main para que leea un archivo de texto (de nuestros datasets por ejemplo) y busque patrones en el mismo.
//se alterara el codigo para en lugar de retornar los lugares en que encuentra el patron en su lugar imprima cuantas veces se encuentra el patron en el texto.

/* C++ Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
using namespace std;
#define NO_OF_CHARS 256
#include "toString.cpp"
class BoyerMoore
{
public:
    // The preprocessing function for Boyer Moore's
    // bad character heuristic
    void badCharHeuristic(string str, int size,
                        int badchar[NO_OF_CHARS])
    {
        int i;

        // Initialize all occurrences as -1
        for (i = 0; i < NO_OF_CHARS; i++)
            badchar[i] = -1;

        // Fill the actual value of last occurrence
        // of a character
        for (i = 0; i < size; i++)
            badchar[(int)str[i]] = i;
    }

    /* A pattern searching function that uses Bad
    Character Heuristic of Boyer Moore Algorithm */
    void search(string txt, string pat, int* count)
    {
        int m = pat.size();
        int n = txt.size();

        int badchar[NO_OF_CHARS];

        /* Fill the bad character array by calling
        the preprocessing function badCharHeuristic()
        for given pattern */
        badCharHeuristic(pat, m, badchar);

        int s = 0; // s is shift of the pattern with
                // respect to text
        while (s <= (n - m)) {
            int j = m - 1;

            /* Keep reducing index j of pattern while
            characters of pattern and text are
            matching at this shift s */
            while (j >= 0 && pat[j] == txt[s + j])
                j--;

            /* If the pattern is present at current
            shift, then index j will become -1 after
            the above loop */
            if (j < 0) {
                //cout << "pattern occurs at shift = " << s
                    // << endl;
                (*count)++; // Increment the count of occurrences
                
                /* Shift the pattern so that the next
                character in text aligns with the last
                occurrence of it in pattern.
                The condition s+m < n is necessary for
                the case when pattern occurs at the end
                of text */
                s += (s + m < n) ? m - badchar[txt[s + m]] : 1;
            }

            else
                /* Shift the pattern so that the bad character
                in text aligns with the last occurrence of
                it in pattern. The max function is used to
                make sure that we get a positive shift.
                We may get a negative shift if the last
                occurrence of bad character in pattern
                is on the right side of the current
                character. */
                s += max(1, j - badchar[txt[s + j]]);
        }
    }
};
/* Driver code */
int main(int argc, char* argv[])
{
    
    if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
    return 0;
    }

    string separador ="$";
    string textoDondeBuscar = toString(argc - 1, &argv[1], separador);
    cout << "Texto concatenado tiene " << textoDondeBuscar.size() << " caracteres." << endl;

    int count = 0;

    // Read each line of the file, store
    // it in string s and print it to the
    // standard output stream 

    string archivoPatrones = argv[argc - 1];
    vector<string> patrones;
    ifstream filePatrones(archivoPatrones);
    if (!filePatrones.is_open()) {
        cerr << "No se pudo abrir el archivo" << archivoPatrones << endl;
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
        int ocurrencias = 0;
        auto start = chrono::high_resolution_clock::now();
        bm.search(textoDondeBuscar, patron, &ocurrencias);
        auto end = chrono::high_resolution_clock::now();
        double running_time = chrono::duration<double>(end - start).count();
        cout << "Terminado: " << patron << endl;
        cout << "El patron \"" << patron << "\" se encuentra " << ocurrencias
            << " veces (en " << running_time << " segundos)" << endl;
    }

    return 0;
}

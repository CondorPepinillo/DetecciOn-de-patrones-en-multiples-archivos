// Rabin-Karp Algorithm for Pattern Searching in C++
// Reference: Introduction to Algorithms (CLRS)
//Implementacion de Rabin-Karp de cortesia de GeeksforGeeks,
//la cual implementaremos junto a nuestro main para que leea un archivo de texto (de nuestros datasets por ejemplo) y busque patrones en el mismo.
//se alterara el codigo para en lugar de retornar los lugares en que encuentra el patron en su lugar imprima cuantas veces se encuentra el patron en el texto.
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

class RabinKarp {
public:
    void search(const string &txt, const string &pat, int *count) {
        
        // Number of characters in the input alphabet (ASCII)
        int d = 256;
        // A prime number for modulo operations to reduce collisions
        int q = 101;
        // Length of the pattern
        int M = pat.length();
        // Length of the text
        int N = txt.length();
        // Hash value for pattern
        int p = 0;
        // Hash value for current window of text
        int t = 0;
        // High-order digit multiplier
        int h = 1;
        
        //vector<int> ans;  se le utiliza para almacenar los indices de las ocurrencias del patron en el texto, pero en este caso no se usa ya que solo se cuenta las ocurrencias.
        
        // Precompute h = pow(d, M-1) % q
        for (int i = 0; i < M - 1; i++){
            h = (h * d) % q;
        }
        // Compute initial hash values for pattern and first window of text
        for (int i = 0; i < M; i++){
            p = (d * p + pat[i]) % q;
            t = (d * t + txt[i]) % q;
        }

        // Slide the pattern over text one by one
        for (int i = 0; i <= N - M; i++){
            // If hash values match, check characters one by one
            if (p == t){
                bool match = true;
                for (int j = 0; j < M; j++){
                    
                    if (txt[i + j] != pat[j]){
                    
                        match = false;
                        break;
                    }
                }
                if (match){
                    //ans.push_back(i + 1);
                    (*count)++; // Increment the count of occurrences
                }
            }

            // Calculate hash value for the next window
            if (i < N - M){
                
                t = (d * (t - txt[i] * h) + txt[i + M]) % q;

                // Ensure hash value is non-negative
                if (t < 0)
                    t += q;
            }
        }
    }
};

int main(){
    ifstream file("datasets/English/english_00"); // Cambia esto por el archivo que desees
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }
    string s;
    
    string pat = "This"; // Cambia esto por el patrón que desees

    int count = 0;

    // Read each line of the file, store
    // it in string s and print it to the
    // standard output stream 

    RabinKarp rk;

    auto start = chrono::high_resolution_clock::now();
    while (getline(file, s))
    {
        rk.search(s, pat, &count);
    }
    auto end = chrono::high_resolution_clock::now();

    double running_time = chrono::duration<double>(end - start).count();
 
    cout << "El patrón '" << pat << "' se encontró " << count << " veces en el archivo en: "<< running_time <<" segundos" << endl;

    file.close();
    return 0;
}
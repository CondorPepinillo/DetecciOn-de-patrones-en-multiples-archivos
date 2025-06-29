#include <bits/stdc++.h>
using namespace std;

int main()
{
    // ...existing code...
    // Leer todo el archivo en un string
    ifstream file("datasets/English/english_00"); // Cambia esto por el archivo que desees
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string txt = buffer.str();

    // Patrón a buscar
    string pat = "This"; // Cambia esto por el patrón que desees

}
